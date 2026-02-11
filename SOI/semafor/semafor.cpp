#include <bits/stdc++.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/stat.h>
#include <sys/shm.h> 
#include "ipcQueue.cpp"






constexpr int NID  = 6;
constexpr int N  = 3;

using namespace std;
const string path = "/tmp";
int nSems = 3;




static struct state { /* Internal record of attached segments. */ 
	int shmid; /* shmid of attached segment */ 
	char *shmaddr; /* attach point */ 
	int shmflg; /* flags used on attach */
} ap; /* State of current attached segments. */


typedef semQueue<qElem, 5> iqueue;
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
	iqueue qB = iqueue(path, NID+10);
    auto semA = getSem(0);
    auto semB = getSem(1);
    auto semM = getSem(2);
	for(int i = 0; i < 20; i++)
	{
		// cerr << "Child\n";
        semM.down();
		// cerr << "Mutex\n";
        semA.down();
		// cerr << "A\n";
        semB.down();
		// cerr << "B\n";

		qElem elemA = qA.get();
		qElem elemB = qB.get();
        semM.up();
		cout << "Child (queue a) got " << elemA.type << " : " << elemA.id << '\n';
		cout << "Child (queue b) got " << elemB.type << " : " << elemB.id << '\n';
		//msleep(1000);
	}
}



void producentA(){	
	iqueue q = iqueue(path, NID);
    auto semA = getSem(0);
	for(int i = 0; i < 10; i++){

		qElem elem({'A', i});
		q.push(elem);
        semA.up();
		//msleep(1);
	}
}

void producentB(){	
	iqueue q = iqueue(path, NID + 10);
    auto semB = getSem(1);
	for(int i = 0; i < 10; i++){
		qElem elem({'B', i});
		q.push(elem);
        semB.up();
		// msleep(100);
	}
}





void (*children[N])() = {*child, *producentA, *producentB};




int main(){
	// auto [semId, shmId] = getSemShm();

	auto q = iqueue(path, NID, true);
	iqueue q2 = iqueue(path, NID+10, true);


	auto semA = getSem(0, true);
    auto semB = getSem(1, true);
    auto semM = getSem(2, true);

	cerr << q.max_size << '\n';


	for(int i = 0; i < N; i++){
		int pid =  fork();
		if (pid < 0) {  
			sys_err("fork err");  
		}

		if(pid == 0){
			children[i]();
			printf("Child %d exitted", i, pid);
			_exit(0);
		}
		else{
			printf("Child %d pid is %d\n", i, pid);
		}
	}
	sleep(20);
	printf("Parent done\n");
	
}