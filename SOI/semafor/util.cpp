#include <bits/stdc++.h>
using namespace std;

void msleep(int milis){
    usleep(milis * 1000);
}

void sys_err(const char *str)  
{  
	perror(str);  
    exit(1);  
}  


union semun {
	int val;
	struct semid_ds *buf;
	unsigned short  *array;
};

