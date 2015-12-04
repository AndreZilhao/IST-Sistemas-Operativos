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

/* lst_iitem_term - each element of the list points to the next element */
typedef struct lst_iitem_term {
   int pid;
   struct lst_iitem_term *next;
} lst_iitem_term;

/* list_term */
typedef struct {
   lst_iitem_term * first;
} list_term;


/* lst_term_new - allocate memory for list_term and initializes it */
list_term* lst_term_new();

/* lst_term_insert -  insert a new item with process id in terminal list */
void lst_term_insert(list_term *list, int pid);

/* lst_term_remove - remove process id from terminal list*/
void lst_te_remove(list_term *list, int pid);

/* lst_term_print - print the terminal */
void lst_term_destroy(list_term *list);

/* lst_term_print - print the terminal */
void lst_term_print(list_term *list);


///----------------------------------------------------///
/* lst_new - allocates memory for list_t and initializes it */
list_t* lst_new();

/* lst_destroy - free memory of list_t and all its items */
void lst_destroy(list_t *);

/* insert_new_process - insert a new item with process id */
void insert_new_process(list_t *list, int pid, time_t starttime);

/* terminate_process - remove first item of value 'value' from list 'list' */
void terminate_process(list_t *list, int pid);

/* check_copy - checks to see if pid exists in list */
int check_copy(list_t *list, int pid);

/* lst_iterate - print the list. */
void lst_print(list_t *list);

/* update_process - updates an item with a return status and the end time of the process. */
void update_process(list_t *list, int pid, int returnType, int returnStatus, time_t endtime);

/* process_exec_time - returns process's execution time */
int process_exec_time(list_t *list, int pid);

