/*
 * list.h - Definitions and declarations of the process list
 */


#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/wait.h>



/* lst_iitem - each element of the list points to the next element */
typedef struct lst_iitem {
   int pid;
   time_t starttime;
   time_t endtime;
   int returnStatus;
   int returnType;
   struct lst_iitem *next;
} lst_iitem_t;

/* list_t */
typedef struct {
   lst_iitem_t * first;
} list_t;



/* lst_new - allocates memory for list_t and initializes it */
list_t* lst_new();

/* lst_destroy - free memory of list_t and all its items */
void lst_destroy(list_t *);

/* insert_new_process - insert a new item with process id */
void insert_new_process(list_t *list, int pid, time_t starttime);

/* lst_remove - remove first item of value 'value' from list 'list' */
void terminate_process(list_t *list, int pid);

/* check_copy - checks to see if pid exists in list */
int check_copy(list_t *list, int pid);

/* lst_iterate - print the list. */
void lst_print(list_t *list);

/* update_process - updates an item with a return status and the end time of the process. */
void update_process(list_t *list, int pid, int returnType, int returnStatus, time_t endtime);

/* process_exec_time - returns process's execution time */
int process_exec_time(list_t *list, int pid);

