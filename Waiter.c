#include <stdlib.h>
#include <stdio.h> 
#include <string.h> 
#include <fcntl.h> 
#include <sys/stat.h> 
#include <sys/types.h> 
#include <sys/wait.h> 
#include <sys/time.h>
#include <unistd.h> 
int main(int argc, char* argv[]){
	char *fifoFileStr1 = "/tmp/fifo_request";
	char *FifoFilestr2 = "/tmp/fifo_release";
	char fifo_requset[Philosopher_Num][20];
	char fifo_release[Philosopher_Num][20];	
	int fd_rquset[Philosopher_Num], fd_release[Philosopher_Num], readable[Philosopher_Num], forks[Philosopher_Num];
	int nfds_rquset, nfds_release , acc_rquset, n_read , ret;	
	int tmp = 0 , n = 0;
	char bin;
	fd_set _request, _release;
	struct timeval mytimevalue;
	
	if (argc!=2){
		printf("Wrong arguments.\n input like this: %s <Philosophers Number>\n", argv[0]);
		fflush(stdout);
		exit(1);
	}	
	int Philosopher_Num = atoi(argv[1]);	
	for(int i=0; i<Philosopher_Num; i++){
		forks[i]=1;
		strcpy(fifo_requset[i], "");
		strcpy(fifo_release[i], "");
		sprintf(fifo_requset[i], "%s%d", fifoFileStr1, i);
		sprintf(fifo_release[i], "%s%d", fifoFileStr2, i);

		if((fd_release[i]  = open(fifo_release[i], O_RDONLY))<0)
		{ 
			perror("Error in Release pipe opening"); 
			exit(fd_release[i]);
		}
		if((fd_rquset[i] = open(fifo_requset[i], O_RDONLY))<0)
		{
			perror("Error Request pipe opening"); 
			exit(fd_rquset[i]);
		}		
	}
	mytimevalue.tv_sec = 1;
	mytimevalue.tv_usec = 0;	
	nfds_release = fd_release[Philosopher_Num-1] + 1;	
	while(1){		
		FD_ZERO(&_release);
		FD_ZERO(&_request);		
		for(int i=0; i<Philosopher_Num; i++){
			FD_SET(fd_release[i], &_release);
			readable[i] = -1;
		}				
		printf("Waiter Release Forks\n"); 
		fflush(stdout);
		if((ret=select(nfds_release, &_release, NULL, NULL, &mytimevalue)) < 0){
			perror("Error in Select on release pipes"); 
			exit(ret); 
		}
		for(int i=0; i < Philosopher_Num; i++){
			if(FD_ISSET(fd_release[i], &_release)){
				read(fd_release[i], &bin, sizeof(char));														
				forks[i] = 1;							
				forks[(i+1)%Philosopher_Num] = 1;
			}
		}		
		printf("ASSIGNMENT\n");
		for(int i=0; i<Philosopher_Num; i++){
			printf("forks %d: %d\n", i, forks[i]);
		}
		fflush(stdout);
		for(int i=0; i<Philosopher_Num; i++){
			if (forks[i] && forks[(i+1)%Philosopher_Num]) 
			{
				FD_SET(fd_rquset[i], &_request); 
				nfds_rquset = fd_rquset[i]+1; 
			}
		}		
		if((ret=select(nfds_rquset, &_request, NULL, NULL, &mytimevalue)) < 0)
		{
			perror("Error in Select on request pipes"); 
			exit(ret); 
		}		
		n_read=0;		
		for(int i=0; i<Philosopher_Num; i++){
			if(FD_ISSET(fd_rquset[i], &_request)){
				readable[n_read++] = i;
			}
		}		
		if (n_read > 0){			
			tmp = rand()%n_read;		
			acc_rquset = readable[tmp];
			if((ret=read(fd_rquset[acc_rquset], &bin, sizeof(char)))<0){
				perror("Error in Read from pipe!");
				exit(ret); 
			}			
			forks[acc_rquset] = 0;
			forks[(acc_rquset+1)%Philosopher_Num] = 0;	
			close(fd_rquset[acc_rquset]);
			if((fd_rquset[acc_rquset] = open(fifo_requset[acc_rquset], O_WRONLY))<0){
				perror("Error In Confirming request"); 
				close(fd_rquset[acc_rquset]);
			}
			bin = 'c';
			if((ret=write(fd_rquset[acc_rquset], &bin, sizeof(char)))<0){
				perror("Error In Read from pipe!"); 
				exit(ret); 
			}
			printf("Accepted Philosopher %d\n", acc_rquset); 
			fflush(stdout);			
			printf("After ASSIGNMENT\n");
			for(int i=0; i<Philosopher_Num; i++){
				printf("forks %d: %d\n", i, forks[i]);
			}			
			close(fd_rquset[acc_rquset]);
			if((fd_rquset[acc_rquset] = open(fifo_requset[acc_rquset], O_RDONLY))<0){ 
				perror("Error In Reopening accepted request!"); 
				exit(fd_rquset[acc_rquset]);
			}
			printf("\n\n\n");						
		}
		sleep(100);
	}
	return 0;
}
