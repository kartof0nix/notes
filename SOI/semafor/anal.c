#include <sys/wait.h>
#include <sys/ptrace.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <lib.h>              
#include <minix/type.h>                     
#include <minix/com.h>
#include <errno.h>


#define MAXN (50000 + 6)
#define n 4
#define STEP  (MAXN)*(MAXN)/1000

int prime[MAXN];

char buf[1024];  


void sys_err(const char *str)  
{  
	perror(str);  
	exit(1);  
}  



void child(int wr_fd, int p2){
	int i;
	int j;
	double p;
	message m;

	if(p2){
		m.m1_i2 = p2;
		_syscall(MM, SETPRI, &m);
		
	}
	for(i = 2; i < MAXN; i++){


		if((i*i) % STEP < ((i-1)*(i-1)) % STEP){
			p = 10.0 * i / MAXN;
			sprintf(buf, "%5f ", p*p);

			write(wr_fd, buf, strlen(buf));  
		}
			

		prime[i]=1;
		for(j = 2; j <= i; j++){
			if(i%j == 0) prime[i] = 0;
		}
	}
}

float progress[n];

void parent(int *pid, int *rd_fd){
	int i;
	int j;

	int    wstatus=0;
	int break_me;
	int reading;
	char c;
	int status;
	int len;
	int st;

	printf("|");
	for(i = 0; i < n; i++){
		printf(" %7s %2d |", "Process", i);
	}
	printf("\n");

	break_me = 0;

	while(1){

		c = ' ';
		printf("|");

		for(i = 0; i < n; i++){
			if(break_me & (1<<i)) continue;

			status = waitpid(*(pid+i), &wstatus, WNOHANG);

			
			if(status != 0){
				printf("\nChild %d is done\n", i);
				progress[i] = 100;
				break_me |= (1<<i);
			}
			for(j = 0; j <= len; j++) buf[i] = 0;
			len = read(*(rd_fd+i), buf, sizeof(buf));  

			if(len > 0){
				buf[len] = 0;
				reading = 1;
				st = 0;
				for(j = len-2; j>=0; j--){
					if(buf[j] == ' '){
						st=j;
						break;
					}
				}

				(sscanf(buf+st, "%f", &progress[i]));
			}

			
		}
		for(i = 0; i < n; i++){

			printf(" %4.1f %%     |", progress[i]);
		}
		printf("\r");
		fflush(stdout);
		if(break_me == (1<<n)-1) break;
	}
}


int main(){
	int i;
	int j;

	int fd[2];  
	int fd_tab[n];  
	int pid_tab[n];
	pid_t pid;
    char p[] = "test for pipe\n";  
          
	
	for(i = 0; i < n; i++){
		
		if (pipe(fd) == -1)   
			sys_err("pipe err"); 
	
		
		pid =  fork();
		if (pid < 0) {  
			sys_err("fork err");  
		}
		fcntl(fd[0], F_SETFD, O_NONBLOCK);
		fcntl(fd[1], F_SETFD, O_NONBLOCK);
		fcntl(fd[0], F_SETFL, O_NONBLOCK);
		fcntl(fd[1], F_SETFL, O_NONBLOCK);

		if(pid == 0){
			close(fd[0]);  /* Close read */
			child(fd[1], 110 * (i==0));
			if(i==0)
				printf("Setting child %d priority to %d", i, 150);
			close(fd[1]);  /* Close wrie */
			_exit(0);
		}
		else{
			close(fd[1]);
			printf("Child %d pid is %d\n", i, pid);
			fflush(stdout);
			pid_tab[i] = pid;
			fd_tab[i] = fd[0];
		}
	}

	/* All processes are open and Im parent. Need to take care of my children */



	

	parent(pid_tab, fd_tab);
	printf("Parent done\n");
	


}
