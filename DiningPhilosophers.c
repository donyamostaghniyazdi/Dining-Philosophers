#include <stdlib.h>
#include <stdio.h> 
#include <string.h> 
#include <fcntl.h> 
#include <sys/stat.h> 
#include <sys/types.h> 
#include <sys/wait.h> 
#include <sys/time.h>
#include <unistd.h> 
#define Philosopher_Num 5
int Seeding(char* exeName, int id) {
	int ret;
	pid_t child_pid = fork();
	if (child_pid != 0)
	{	
		return child_pid;
	}
	else 
	{
		char tmp[5]="";
		sprintf(tmp, "%d", id);
		char * args[] = { "/usr/bin/konsole",  "-e", exeName, tmp, (char*)NULL };
		ret = execvp(args[0], args);
		perror("execution failed!");
		exit(ret);
	}
}

int CleanLast(){
	char tmp[5];
	int ret;
	sprintf(tmp, "%d", PH_NUM);
	char *args[] = {"./clean_tmp_fifo", tmp, NULL};
	int pid = fork();
	if (pid<0){
		perror("Error in Child Clean");
		return pid;
	}
	if (pid==0){
		ret = execvp(args[0], args);
		perror("Error in Execution Clean");
		return ret;
	}
	return pid;
}

int main(int argc, char *argv[]){
	int clean_pid;	
	pid_t philosopher_pid[Philosopher_Num];
	pid_t waiter_pid;	
	char *fifoFileStr1 = "/tmp/fifo_request";		
	char *fifoFileStr2 = "/tmp/fifo_release";	
	char *philosopher_str = "./philosopher";
	char *waiter_str = "./waiter";
	char fifo_request[Philosopher_Num][128];
	char fifo_release[Philosopher_Num][128];
	if((clean_pid=CleanLast())<0){
		perror("Error in Clean!\n");
		exit(clean_pid);
	}	
	waitpid(clean_pid, NULL, 0);
	printf("Cleaning FIFO that lastly existed\n"); 
	fflush(stdout);
	if((waiter_pid = Seeding(waiter_str, Philosopher_Num))<0){
		perror("Generates a waiter");
		exit(waiter_pid);
	}
	for(int i=0; i<Philosopher_Num; i++){
		strcpy(fifo_request[i], "");
		strcpy(fifo_release[i], "");
		sprintf(fifo_request[i], "%s%d", fifoFileStr1, i);
		sprintf(fifo_release[i], "%s%d", fifoFileStr2, i);
		if (mkfifo(fifo_request[i], 0666))
			perror("Can't create a fifo for request");
		if (mkfifo(fifo_release[i], 0666))
			perror("Can't create a fifo for release");		
		if((philosopher_pid[i] = Seeding(philosopher_str, i))<0){
			perror("Error in Generates a Philosopher"); 
			for(int j=0; j<i; j++){
				waitpid(philosopher_pid[j], NULL, 0); 
			}
			return 1;
		}
	}
	return 0;
}




