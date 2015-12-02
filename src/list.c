/*
 * list.c - A process ID and Return values list.
 */


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "list.h"
#include <sys/wait.h>
#include <time.h>



 list_t* lst_new()
 {
 	list_t *list;
 	list = (list_t*) malloc(sizeof(list_t));
 	list->first = NULL;
 	return list;
 }


 void lst_destroy(list_t *list)
 {
 	struct lst_iitem *item, *nextitem;

 	item = list->first;
 	while (item != NULL)
 	{
 		nextitem = item->next;
 		free(item);
 		item = nextitem;
 	}
 	free(list);
 }


 void insert_new_process(list_t *list, int pid, time_t starttime)
 {
 	lst_iitem_t *item;
 	item = (lst_iitem_t *) malloc (sizeof(lst_iitem_t));
 	item->pid = pid;
 	item->starttime = starttime;
 	item->endtime = 0;
 	item->returnType = 0;
 	item->returnStatus = 0;
 	item->next = list->first;
 	list->first = item;
 }


 void terminate_process(list_t *list, int pid)
 {
 	lst_iitem_t *item, *nextitem;
 	item = list->first;
 	while (item != NULL)
 	{
 		if(item->next != NULL)
 		{
 			if (item->next->pid == pid)
 			{
 				nextitem = item->next;
 				free(nextitem);
 				item->next = item->next->next;
 				printf("terminated process with pid: %d\n", pid);
 				return;
 			}
 			item = item->next;
 		}
 	}
 }

 int check_copy(list_t *list, int pid)
 {
 	lst_iitem_t *item;
 	item = list->first;
 	while (item != NULL)
 	{
 		if (item->pid == pid)
 		{
 			return 1;
 		}
 		item = item->next;
 	}
 	return 0;
 }


 void lst_print(list_t *list)
 {
 	lst_iitem_t *item;
 	item = list->first;
 	printf("---\n");
 	while (item != NULL)
 	{
 		printf("Process ID: %d | ", item->pid);
		printf("Execution Time: %d sec | ", (int) difftime(item->endtime, item->starttime));
		
 		if(item->returnType == 0)
 		{
 			printf("Return Integer:%d\n", item->returnStatus);
 		}
 		if(item->returnType == 1)
 		{
 			printf("Return Integer:%d [SIGNAL]\n", item->returnStatus);
 		}
 		item = item->next;
 	}
 }


 int process_exec_time(list_t *list, int pid)
 {
	int finaltime = 0;
	lst_iitem_t *item;
 	item = list->first;
 	while(item != NULL)
 	{
 		if (item->pid == pid)
 		{
			finaltime = (int) difftime(item->endtime, item->starttime);
 			return finaltime;
 		}
 		item = item->next;
 	}
	return -1;
 }

 void update_process(list_t *list, int pid, int returnType, int returnStatus, time_t endtime)
 {
 	lst_iitem_t *item;
 	item = list->first;
 	while(item != NULL)
 	{
 		if (item->pid == pid)
 		{
 			item->returnType = returnType;
 			item->returnStatus = returnStatus;
 			item->endtime = endtime;
 			return;
 		}
 		item = item->next;
 	}
 }
