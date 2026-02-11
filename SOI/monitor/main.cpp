#include <bits/stdc++.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/stat.h>
#include <sys/shm.h> 
#include "monitor.cpp"

#include <signal.h>




constexpr int NID  = 32;
constexpr int N  = 3;

using namespace std;
const string path = "/tmp";
int nSems = 3;




static struct state { /* Internal record of attached segments. */ 
	int shmid; /* shmid of attached segment */ 
	char *shmaddr; /* attach point */ 
	int shmflg; /* flags used on attach */
} ap; /* State of current attached segments. */


typedef monQueue <qElem, 5> iqueue;

struct Main{

	Monitor mon;

	Monitor::condition a_empty, b_empty, a_full, b_full;

	Main(string path, const int seed) : mon(path, seed+7, true), a_empty(mon.semid, 4, 0), b_empty(mon.semid, 5, 0), a_full(mon.semid, 6, 0), b_full(mon.semid, 7, 0)  { };
	static int getMySem(string path, const int sid, const int semNum)
	{
		semun ctlarg;
		key_t key;
		int semid;
		int shmid; /* return value from shmget() */
		sembuf sbuf;

		if ((key = ftok(path.c_str(), sid)) == (key_t)-1)
		{
			sys_err("IPC error: ftok");
		}
		if ((semid = semget(key, semNum, 0)) == -1)
		{
			/* Semaphore does not exist - Create. */
			if ((semid = semget(key, semNum, IPC_CREAT | S_IRWXU | 0666 )) <= 0)
			{
				sys_err(("Semget err " + to_string(semid)).c_str());
			}
		}

		return semid;
	}
	semaphore getSem(int id, bool init = false){
		int v0[3] = {0, 0, 1}; //SemA, semB, Mutex
		auto semid = getMySem(path, NID+5, 3);
		if(init) return semaphore(semid, id, v0[id]);
		return semaphore(semid, id);
	}

	void child(){
		
		iqueue qA = iqueue(path, NID);
		iqueue qB = iqueue(path, NID+3);
		auto semA = getSem(0);
		auto semB = getSem(1);
		auto semM = getSem(2);
		for(int i = 0; i < 20; i++)
		{
			mon.enter();
			
			if(qA.getSize() == 0){
				mon.wait(a_empty);
			}

			if(qB.getSize() == 0){
				mon.wait(b_empty);
			}

			
			qElem elemA = qA.get();
			qElem elemB = qB.get();

			cout << "Child (queue a) got " << elemA.type << " : " << elemA.id << endl;
			cout << "Child (queue b) got " << elemB.type << " : " << elemB.id << endl;
			if(qA.getSize() == qA.max_size-2)
				mon.signal(a_full);
			if(qB.getSize() == qB.max_size-2)
				mon.signal(b_full);

			mon.exit();
		}
	}
	void producentA(){	
		iqueue q = iqueue(path, NID);
		auto semA = getSem(0);
		for(int i = 0; i < 10; i++){
			mon.enter();

			if(q.getSize() == q.max_size-1)
				mon.wait(a_full);

			qElem elem({'A', i});
			q.push(elem);

			if(q.getSize() == 1){
				mon.signal(a_empty);
			}
			mon.exit();

			//msleep(1);
		}
	}
	
	void producentB(){	
		iqueue q = iqueue(path, NID + 3);
		auto semB = getSem(1);
		for(int i = 0; i < 10; i++){
			mon.enter();

			if(q.getSize() == q.max_size-1)
				mon.wait(b_full);

			qElem elem({'B', i});

			q.push(elem);
			
			if(q.getSize() == 1){
				mon.signal(b_empty);
			}
			
			// msleep(100);
			mon.exit();
		}
	}

};






Main prog(path, NID+6);



int main(){
	// auto [semId, shmId] = getSemShm();
	cerr << "Main0\n";
	auto q = iqueue(path, NID, true);
	iqueue q2 = iqueue(path, NID+3, true);
	cerr << "Main1\n";


	auto semA = prog.getSem(0, true);
    auto semB = prog.getSem(1, true);
    auto semM = prog.getSem(2, true);

	cerr << q.max_size << '\n';
	cerr << "Main2\n";

	vector<int> pids;

	for(int i = 0; i < N; i++){
		int pid =  fork();
		if (pid < 0) {  
			sys_err("fork err");  
		}

		if(pid == 0){
			switch(i){
				case 0:
					prog.child();
					break;
				case 1:
					prog.producentA();
					break;
				case 2:
					prog.producentB();
					break;
			}
			printf("Child %d exitted", i, pid);
			_exit(0);
		}
		else{
			printf("Child %d pid is %d\n", i, pid);
			pids.push_back(pid);
		}
	}
	sleep(4);
	for(int i : pids){
		kill(i, SIGINT);
		sleep(1);
		kill(i, SIGKILL);
	}
	printf("Parent done\n");
	
}