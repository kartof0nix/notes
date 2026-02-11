#include <bits/stdc++.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/stat.h>
#include <sys/shm.h>
#include "util.cpp"

using namespace std;

class semaphore
{
	semun ctlarg;

	int semid, semnum;

	sembuf sb[2] = {
		{0, 1, 0},
		{0, -1, 0}};

public:
	semaphore() : semid(0), semnum(0) {};

	semaphore(int sem_id, int sem_num, int v0 = -1) : semid(sem_id), semnum(sem_num)
	{
		sb[0].sem_num = sem_num;
		sb[1].sem_num = sem_num;
		ctlarg.val = v0;
		if (v0 != -1 && semctl(semid, semnum, SETVAL, ctlarg) == -1)
		{
			sys_err("IPC error: semctl");
		}
	}
	semaphore& operator=(const semaphore &b){
		semid = b.semid;
		semnum = b.semnum;
		return *this;
	}
	const void up()
	{
		if (int e = semop(semid, sb, 1) != 0)
		{
			sys_err(("Semop error " + to_string(errno)).c_str());
		}
	}
	const void down()
	{
		if (int e = semop(semid, sb + 1, 1) != 0)
		{
			sys_err(("Semop error " + to_string(errno)).c_str());
		}
	}
};

struct qElem
{
	char type;
	int id;
};

template <typename elem_t, size_t QLEN>
struct rawShmQueue
{
	/**
	 * Memory structure
	 * 0-3 : offset
	 * 4-7 : *qBack
	 * 8-. : qElem[]
	 */
	constexpr static size_t max_size = QLEN;

protected:
	constexpr static size_t elem_sz = sizeof(elem_t);
	const static size_t buf_size = QLEN * elem_sz + 2 * sizeof(int *);

private:
	char *p;
	int *pFrontIdx;
	int *pBackIdx;
	elem_t *qBufStart;
	elem_t *qBufEnd;
public:

	rawShmQueue(void *base, bool clear = false) :
		p(reinterpret_cast<char*>(base)),
		pFrontIdx(reinterpret_cast<int*>(p)),
		pBackIdx(reinterpret_cast<int*>(p + sizeof(int))),
		qBufStart(reinterpret_cast<elem_t*>(p + 2 * sizeof(int))),
		qBufEnd(qBufStart + QLEN)
		{
			if (base == (void*)-1 || base == nullptr) {
				sys_err("Shmat error!");
			}

			if (clear) {
				*pFrontIdx = 0;
				*pBackIdx  = 0;
			}
	}

	int getSize(){
		return (*pBackIdx - *pFrontIdx + QLEN) % QLEN;
	}


public:
	elem_t qGet()
	{
		//cerr << "Before get: pBackIdx = " << *pBackIdx << ", pFrontIdx = " << *pFrontIdx << endl;
		int f = *pFrontIdx;
		elem_t el = qBufStart[f];
		f = (f + 1) % QLEN;
		(*pFrontIdx) = f;
		//cerr << "After get: pBackIdx = " << *pBackIdx << ", pFrontIdx = " << *pFrontIdx << endl;
		return el;
	}

	void qPush(const elem_t &elem)
	{
		//cerr << "Before push: pBackIdx = " << *pBackIdx << ", pFrontIdx = " << *pFrontIdx << endl;
		int b = *pBackIdx;
		qBufStart[b] = elem;
		b = (b + 1) % QLEN;
		(*pBackIdx) = b;
		//cerr << "After push: pBackIdx = " << *pBackIdx << ", pFrontIdx = " << *pFrontIdx << endl;
	}

	/////////////////////// IPC INIT FUNCTIONS ////////////////////////////

	
	// public:
	// static rawShmQueue init(string key, int id, bool clear = false)
	// {
	// 	void* base = shmat(shmid, NULL, 0);
	// 	if (base == (void*)-1) sys_err("shmat qFront");

	// 	return ipcQueue(base, clear);
	// }
};

struct Monitor{
	typedef semaphore condition;
	key_t key= 0;
	int semid;
	condition mutex;

	static constexpr int NSEMS = 10;

	static key_t getKey(string path, const int seed){
		key_t key;
		if ((key = ftok(path.c_str(), seed)) == (key_t)-1)
		{
			sys_err("IPC error: ftok");
		}
		return key;
	}
	static int getSemId(key_t key, const int semNum)
	{
		int semid;
		if ((semid = semget(key, semNum, 0)) == -1)
		{
			/* Semaphore does not exist - Create. */
			if ((semid = semget(key, semNum, IPC_CREAT | S_IRWXU | 0666 )) <= 0)
			{
				sys_err(("Semget err " + to_string(key) + " " + to_string(semNum) ).c_str());
			}
		}
		return semid;
	}
	Monitor(string path, int seed, bool enter) : key(getKey(path, seed)), semid(getSemId(key, NSEMS)){
		mutex = condition(semid, 0, enter ? 1 : -1);
	}
	void enter(){
		mutex.down();
	}
	void exit(){
		mutex.up();
	}
	void wait(condition c){
		mutex.up();
		c.down();
		mutex.down();
	}

	void signal(condition c){
		c.up();
	}

};

template <typename elem_t, size_t QLEN>
struct monQueue : rawShmQueue<elem_t, QLEN>
{
	const key_t key = 0;
	Monitor mon;
	const int semid;
	const int shmid;
	Monitor::condition c_empty, c_full;
	typedef rawShmQueue<elem_t, QLEN> raw_q ;
	
	monQueue(string path, const int seed, bool clear = false):
	raw_q(shmat(getShmId(mon.getKey(path, seed), raw_q::buf_size), NULL, 0), clear),
		key(mon.getKey(path, seed)),
		mon(path, seed, clear),
		semid(mon.getSemId(key, mon.NSEMS)),
		shmid(getShmId(key, raw_q::buf_size)),
		c_empty(semid, 1, clear ? 0 : -1),
		c_full(semid, 2, clear ? 0 : -1)
		{
		};
		
	qElem get()
	{
		mon.enter();
		
		if(raw_q::getSize() == 0)
			mon.wait(c_empty);
		
		auto res = raw_q::qGet();
		if(raw_q::getSize() == QLEN - 2)
			mon.signal(c_full);

		mon.exit();
		return res;
	}
	void push(qElem &elem)
	{

		mon.enter();

		if(raw_q::getSize() == QLEN-1)
			mon.wait(c_full);
		

		raw_q::qPush(elem);

		if(raw_q::getSize() == 1)
			mon.signal(c_empty);
		mon.exit();
	}
	
	static int getShmId(key_t key, const int ShmSz)
	{
		int shmid; /* return value from shmget() */
		if ((shmid = shmget(key, ShmSz, IPC_CREAT | 0666)) == -1)
		{
			sys_err("shmget: shmget failed");
		}
		
		return shmid;
	}
};


