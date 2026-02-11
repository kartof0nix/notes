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

	const int semid, semnum;

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


public:
	elem_t qGet()
	{
		int f = *pFrontIdx;
		elem_t el = qBufStart[f];

		f = (f + 1) % QLEN;
		(*pFrontIdx) = f;

		return el;
	}

	void qPush(const elem_t &elem)
	{
		int b = *pBackIdx;
		//cerr << "Push " << elem.type << ' ' << pBackIdx << '\n';
		qBufStart[b] = elem;
		b = (b + 1) % QLEN;
		(*pBackIdx) = b;
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


template <typename elem_t, size_t QLEN>
struct semQueue : rawShmQueue<elem_t, QLEN>
{
	typedef rawShmQueue<elem_t, QLEN> raw_q ;
	const key_t key;
	constexpr static int NSEMS = 4;
	const int semid;
	const int shmid;

	enum semids
	{
		MUTEX_GET,
		MUTEX_PUT,
		EMPTY,
		FULL
	};

	semaphore sem[4];

	static key_t getKey(string path, const int seed){
		key_t key;
		if ((key = ftok(path.c_str(), seed)) == (key_t)-1)
		{
			sys_err("IPC error: ftok");
		}
		return key;
	}
	static int getShmId(key_t key, const int ShmSz)
	{
		int shmid; /* return value from shmget() */
		if ((shmid = shmget(key, ShmSz, IPC_CREAT | 0666)) == -1)
		{
			sys_err("shmget: shmget failed");
		}
		
		//cerr << "Shmid: " << key << ' ' << ShmSz << ' ' << shmid << '\n';
		return shmid;
	}
	static int getSemId(key_t key, const int semNum)
	{
		int semid;
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

	semQueue(string path, const int seed, bool clear = false):
		key(getKey(path, seed)),
		semid(getSemId(key, NSEMS)),
		shmid(getShmId(key, raw_q::buf_size)),
		sem{semaphore(semid, 0, (clear ? 1 : -1)), 
			semaphore(semid, 1, (clear ? 1 : -1)), 
			semaphore(semid, 2, (clear ? 0 : -1)), 
			semaphore(semid, 3, (clear ? QLEN : -1))
		},
		raw_q(shmat(getShmId(getKey(path, seed), raw_q::buf_size), NULL, 0), clear) {
		};

	qElem get()
	{
		sem[EMPTY].down();
		sem[MUTEX_GET].down();
		auto res = raw_q::qGet();
		sem[MUTEX_GET].up();
		sem[FULL].up();
		return res;
	}
	void push(qElem &elem)
	{
		sem[FULL].down();
		sem[MUTEX_PUT].down();
		raw_q::qPush(elem);
		sem[MUTEX_PUT].up();
		sem[EMPTY].up();
	}
};

