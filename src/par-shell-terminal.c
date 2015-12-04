#include <stdio.h>
#include <fcntl.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <semaphore.h>
#include <sys/stat.h>
#include "list.h"
#include "commandlinereader.h"
#define PIPEOUT "pipe-out-"
#define MAXLINE 255 //Linux Maximum Filename Length

char * getAline(void);
int sendMessage(char * message, int fd);


int main(int argc, char *argv[])
{
	/*Redirecting Stdin to Pipe*/
	int pipeFd;
	char * input;
	char * output;
	char firstArgument[MAXLINE];
	char localPid[8];
	char message[14];
	snprintf(localPid, 8, " %d\n", (int) getpid());

	if (argc != 2)
	{ 
		printf("Incorrect arguments: Exiting.\n");
		exit(EXIT_FAILURE);
	}
	pipeFd = open(argv[1], O_RDWR);
	if (pipeFd < 0)
	{ 
		printf("Could not open Pipe.\n");
		exit(EXIT_FAILURE);
	}
	//Signal Par-shell that a new terminal has been opened.
	snprintf(message, 14, "[new]");
	output = strcat(message, localPid);
	if(sendMessage(output, pipeFd) == -1) return -1;


	while(1)
	{
		input = getAline();
		puts(input);
		sscanf(input,"%s",firstArgument);
		puts(firstArgument);
		printf("%ld\n", sizeof(firstArgument));

		/*
		"exit"
		If "exit" is found, two iterations of the process list are run. The first one calls "waitpid()" on all child processes,
		and the second one prints the Process ID and return value.
		*/
		

		
		if(NULL == firstArgument)
		{
			continue;
		}

		if(strcmp ("exit", firstArgument) == 0)
		{
			//Signal Par-shell that this terminal has closed.
			snprintf(message, 14, "[del]");
			output = strcat(message, localPid);
			if(sendMessage(output, pipeFd) == -1) return -1;
			close(pipeFd);
			exit(EXIT_SUCCESS);

		}

		if(strcmp ("stats", firstArgument) == 0)
		{
			output = strcat(firstArgument,localPid);
			if(sendMessage(output, pipeFd) == -1) return -1;


			char parShellOut[20] = PIPEOUT;
			//strcat(parShellOut, pidChar);
			int returnpipe = mkfifo(parShellOut, S_IRWXU);
			printf("The mkfifo() call returned %d\n", returnpipe);
			continue;
		}
		if(sendMessage(input, pipeFd) == -1) return -1;
	}
}

int sendMessage (char * input, int fd)
{
	int writeResult = write(fd,input,strlen(input));
	if(writeResult != strlen(input)) 
	{	
		printf("Size: %d\t", (int)strlen(input));
		printf("Written: %d\t", writeResult);
		perror("Failed to write string to the pipe.\n");
		return -1;
	}
	return 0;
}

char * getAline(void) {
	char * line = malloc(100), * linep = line;
	size_t lenmax = 100, len = lenmax;
	int c;

	if(line == NULL)
		return NULL;

	for(;;) {
		c = fgetc(stdin);
		if(c == EOF)
			break;

		if(--len == 0) {
			len = lenmax;
			char * linen = realloc(linep, lenmax *= 2);

			if(linen == NULL) {
				free(linep);
				return NULL;
			}
			line = linen + (line - linep);
			linep = linen;
		}

		if((*line++ = c) == '\n')
			break;
	}
	*line = '\0';
	return linep;
}