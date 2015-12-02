/* 
- Andre Zilhao
- Rita Rocha
- Eduardo Monteiro
@ ist.utl.pt 2015
Operating Systems - Part 4
Program: par-shell
Description: C program that emulates a shell. Has the capability to run multiple programs at the same time on a multi-core machine.
Uses a list to keep track of all processes as well as return values. Acepts up to 5 optional arguments.
Input is as follows:

pathname [arg1 arg2 ...] 
- Where pathname is the path to the program to be run, and arg1,arg2,... are optional arguments.

exit
- Exits the program. Waits for all open processes to finish and then displays their process ID's and return values.
*/
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <semaphore.h>
#include "list.h"
#include "commandlinereader.h"
#define MAXLINE 40
#define MAXPAR 2

int numChildren;
int exitcalled;
int numIter;
int execTime;
char logline[MAXLINE];
list_t *pidList;
pthread_mutex_t mutex;
pthread_cond_t condMonitor;
pthread_cond_t condMaxpar;
FILE * log;
//sem_t threadSem;
//sem_t monitorSem;


void log_analizer()
{
	while (fgets(logline, MAXLINE, log) != NULL)
	{
		if (logline[0] == "i")
			sscanf( logline, "iteracao %d", &numIter);
		
		else (logline[0] == "t")
			sscanf( logline, "total execution time: %d", &execTime);
	}
}

void log_filho(int filhoPID)
{
	int filhoExecTime = process_exec_time(pidList, filhoPID);
	execTime = execTime + filhoExecTime;
	
	fprintf(log, "iteracao %d\n", numIter);
	fprintf(log, "pid: %d execution time %d s\n", filhoExecTime);
	fprintf(log, "total execution time: %d s\n", execTime);
	fflush(log);
}


/*Function to be run by monitor thread*/
void* monitoriza()
{
	int status;
	int pid;
	
	log_analizer;
	
	while(1)
	{

		//sem_wait(&monitorSem);
		pthread_mutex_lock(&mutex);
		while (numChildren == 0 && exitcalled == 0)
		{
			pthread_cond_wait(&condMonitor, &mutex);
		}
		
		pthread_mutex_unlock(&mutex);
		pid = wait(&status);
		if(WIFEXITED(status))
		{
			pthread_mutex_lock(&mutex);
			update_process(pidList, pid, 0, WEXITSTATUS(status), time(NULL));
			log_filho(pid);
			pthread_mutex_unlock(&mutex);
		}
		if(WIFSIGNALED(status))
		{
			pthread_mutex_lock(&mutex);
			update_process(pidList, pid, 1, WIFSIGNALED(status), time(NULL));
			log_filho(pid);
			pthread_mutex_unlock(&mutex);
		}
		numChildren--;
		pthread_cond_signal(&condMaxpar);
			//sem_post(&threadSem);
		if (exitcalled == 1 && numChildren == 0) 
		{
			pthread_exit(NULL);
		}

	}
	return NULL;
}

int main(int argc, char *argv[])
{

  /*Variables:
  argNR = Number of Variable Arguments, pid = Process ID, lineArgs = Input String, pidList = Process List.*/
	int argNr = 6, pid;
	char * lineArgs[argNr];
	pidList = lst_new();
	exitcalled = 0;
	numChildren = 0;
	numIter = 0;
	execTime = 0;
	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init(&condMonitor, NULL);
	pthread_cond_init(&condMaxpar, NULL);
	//sem_init(&threadSem, 0, MAXPAR);
	//sem_init(&monitorSem, 0, 0);

  /*Creation of monitor thread*/
	pthread_t monitor;
	pthread_create(&monitor, 0, monitoriza, NULL);
	
	/*a+ opens file for reading at beguining of file*/
	/*and for appending (AKA writing at end of file)*/
	/*if file does not exist, it creates the file*/
	log = fopen ("log.txt", "a+");
	if(log == NULL)
	{
      perror("Error opening file");
      return(-1);
	}
	
  /*Input Loop:
  Constantly reads inputs from the keyboard with "readLineArguments" function call, and processes
  the input into either "exit", or "pathname".*/

  while(1)
  {
  	if(readLineArguments(lineArgs,argNr+1) == -1)
  	{
  		continue;
  	}
    /*"exit"
    If "exit" is found, two iterations of the process list are run. The first one calls "waitpid()" on all child processes,
    and the second one prints the Process ID and return value.*/
    
    if(NULL == lineArgs[0])
    {
    	continue;
    }

    if(strcmp ("exit", lineArgs[0]) == 0)
    {
    	exitcalled = 1;
    	pthread_cond_signal(&condMaxpar);
    	pthread_cond_signal(&condMonitor);
    	pthread_join(monitor, NULL);
    	lst_print(pidList);
		fclose(log);
    	lst_destroy(pidList);
    	pthread_mutex_destroy(&mutex);
    	pthread_cond_destroy(&condMonitor);
    	pthread_cond_destroy(&condMaxpar);
		//sem_destroy(&threadSem);
		//sem_destroy(&monitorSem);
    	exit(EXIT_SUCCESS);
    }
    /*"pathname"
    If anything but "exit" is found, a new child process is opened with "fork()", and if it's the child, "execv()" is run.
    If it's the father, it will add the new child's process ID to the list.*/
    else 
    {
    	pthread_mutex_lock(&mutex);
		while (numChildren >= MAXPAR)
		{
			pthread_cond_wait(&condMaxpar, &mutex);
		}
		pthread_mutex_unlock(&mutex);	
    	pid = fork();
    	if (pid == 0)
    	{		
    		execv(lineArgs[0], lineArgs);
    		perror("ERROR");
    		exit(EXIT_FAILURE);
    	}
    	if (pid < 0)
    	{
    		perror("ERROR");
    		continue;
    	}
    	else
    	{

    		pthread_mutex_lock(&mutex);
    		numChildren++;
    		insert_new_process(pidList, pid, time(NULL));
    		pthread_mutex_unlock(&mutex);
    		pthread_cond_signal(&condMonitor);
			//sem_post(&monitorSem);
    		continue;
    	}

    }

}
return 0;
}