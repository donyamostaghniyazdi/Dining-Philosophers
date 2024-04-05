#include <stdlib.h>
#include <stdio.h> 
#include <string.h> 
#include <fcntl.h> 
#include <sys/stat.h> 
#include <sys/types.h> 
#include <sys/wait.h> 
#include <unistd.h> 
int main(int argc, char* argv[]){	
	char *fifoFileStr1 = "/tmp/fifo_request";
	char *FifoFilestr2 = "/tmp/fifo_release";
	char fifo_requset[128] , fifo_release[128];	
	int fd_requset, fd_rls , ret;
	char bin;	
	unsigned long int step = 0;	
	if (argc != 2){
		printf("Wrong Argument! \nThe Template should like: %s <id>\n", argv[0]);
		exit(1);
	}	
	int id = atoi(argv[1]);	
	printf("Philosopher %d has spawned with PID %d\n", id, getpid());	
	strcpy(fifo_requset, "");
	sprintf(fifo_requset, "%s%d", fifoFileStr1, id);
	if((fd_requset = open(fifo_requset, O_WRONLY))<0){
		perror("Error in Request Pipe Opening!"); 
		exit(fd_requset); 
	}	
	strcpy(fifo_release, "");	
	sprintf(fifo_release, "%s%d", fifoFileStr2, id);
	if((fd_rls  = open(fifo_release,  O_WRONLY))<0){
		perror("Error in Release Pipe Opening!"); 
		exit(fd_rls);
	}
	printf("Philosopher %d has Set\n", id); 
	fflush(stdout);	
	while(1){		
		bin = 'c';		
		printf("%d Is standby!\n", id);
		usleep(rand()%2000 * 1000);	
		printf("%d Asking For Forks.\n", id);
		if(write(fd_requset, &bin, sizeof(char)) < 0) 
		    perror("PH write");
		printf("%d Waiting For Forks\n", id); 
		fflush(stdout);
		close(fd_requset);		
		if((fd_requset = open(fifo_requset, O_RDONLY)) < 0){ 
			perror("Error In Request pipe opening!"); 
			exit(fd_requset); 
		}		
		if((ret=read(fd_requset, &bin, sizeof(char))) < 0){
			perror("Error in PH write"); 
			exit(ret); 
		}
		close(fd_requset);
		if((fd_requset = open(fifo_requset, O_WRONLY)) < 0){ 
			perror("Error in Request pipe opening!"); 
			exit(fd_requset); 
		}
		printf("%d Eating!, %lu!\n", id, step++); 
		fflush(stdout);
		usleep(rand()%3000 * 1000);
		printf("%d Release Fork!\n", id);
		if((ret=write(fd_rls, &bin, sizeof(char))) < 0){
			perror("Error in PH Write Chopstick Release"); 
			exit(ret); 
		}		
	}	
	return 0;
}
