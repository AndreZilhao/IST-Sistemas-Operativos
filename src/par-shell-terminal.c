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
#define MAXLINE 40

char * getAline(void);


int main(int argc, char *argv[])
{
	/*Redirecting Stdin to Pipe*/
	int pipeFd, writeResult;
	char * input;
	char firstArgument[MAXLINE];

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
	while(1)
	{
		input = getAline();
		puts(input);
		sscanf(input,"%s",firstArgument);
		puts(firstArgument);
		printf("%ld\n", sizeof(firstArgument));

		/*"exit"
		If "exit" is found, two iterations of the process list are run. The first one calls "waitpid()" on all child processes,
		and the second one prints the Process ID and return value.*/
		

		
		if(NULL == firstArgument)
		{
			continue;
		}

		if(strcmp ("exit", firstArgument) == 0)
		{
			close(pipeFd);
			exit(EXIT_SUCCESS);

		}

		if(strcmp ("stats", firstArgument) == 0)
		{
			exit(EXIT_SUCCESS);
		}
		
		writeResult = write(pipeFd, input, strlen(input));
		if(writeResult != strlen(input)) 
		{	
			printf("Size: %d\n", (int)strlen(input));
			printf("Written:: %d\n", writeResult);
			perror("Failed to write string to the pipe.\n");
			return -1;
		}
	}
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