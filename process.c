#include "3140_concur.h"
#include <stdlib.h>
#include <fsl_device_registers.h>

/* PCB process control block */
struct process_state {
	unsigned int *sp;
	int size;
	struct process_state *next_process;
	/* the stack pointer for the process */
};

/* current process is NULL before scheduler starts and after it end */
process_t * current_process = NULL;

/* make/update process queue? linked list */
struct process_queue {
	struct process_state *val;
	struct process_queue *next;
};

struct process_queue *process_one = NULL; // initialize queue's global variable keeping track of first process in queue

void add_to_queue (struct process_queue *new_process) 
{
	// list is empty, add to beginning
	if (process_one == NULL) {
		process_one = new_process;
		new_process->next = NULL;
	} 
	// list is not empty, add to end
	else {
		struct process_queue *tmp = process_one;
		// traverse the list to the end
		while (tmp->next != NULL) {
			tmp = tmp->next;
		}
		// add new_process as the next of the current last
		tmp->val->next_process = new_process->val;
		tmp->next = new_process;
		new_process->next = NULL;
	}
};

/* Remove first element from queue; we only ever remove first element of list
When first element is only element, next is null. Therefore, the first element is null */
void remove_from_queue (void) {
	if (process_one != NULL) process_one = process_one->next;
};


/* Creates a new process that starts at function f, initial stack size n 
Returns 0 on success, -1 if error 
May require allocating memory for a process_t structure, use malloc() 
Should called process_stack_init() to allocate the stack 
Should allocate a process_t 
Should add the process to your scheduler's data structures */
int process_create (void (*f) (void), int n)
{
	unsigned int *sp = process_stack_init((*f), n);
	if (*sp == NULL) return -1;
	process_t *process = malloc(sizeof(process_t));
	process->sp = sp;
	process->size = n;
	process->next_process = NULL;
	struct process_queue *new_process;
	new_process->val = process;
	add_to_queue(new_process);
	return 0;
};

/* Start process with the given amount of time
*/
void process_start (void)
{
	PIT->MCR = 0;
	PIT->CHANNEL[0].LDVAL = 0x1E8480; // ~10 Hz, load 2 million cycles
	NVIC_EnableIRQ(PIT0_IRQn);
	current_process = process_one->val;
	process_begin();
}



/* updates process_t
process_t is NULL until process_start is called
process_t is also NULL when a process terminates
current_process is process_t
cursp is null if no process is running or when process just finished */
unsigned int * process_select(unsigned int * cursp)
{
	if (cursp == NULL) {
		// remove first process in queue if it exists
		// free the stack memory
		// save process in a temp variable
		struct process_state *tmp = current_process;
		// free the stack for the previous process and remove from the queue
		process_stack_free(tmp->sp, tmp->size);
		remove_from_queue();
		// update current_process; null if queue is empty
		current_process = process_one->val;
		return current_process->sp;
		
		/*
		// nothing was running
		if (current_process == NULL) {
			// queue is empty, return null
			if (process_one == NULL) return NULL;
			// queue is not empty, return the first process of the queue's SP
			else{
				current_process = process_one->val;
				return current_process->sp;
			}
		}
		// just finished process
		else {
			// save process in a temp variable
			struct process_state *tmp = current_process;
			// update current_process
			current_process = current_process->next_process;
			// free the stack for the previous process and remove from the queue
			process_stack_free(tmp->sp, tmp->size);
			remove_from_queue();
			return current_process->sp;
		}
		*/
	}
	// process was not done
	else {
		struct process_queue *tmp = process_one;
		// kick current process to the back of the queue and remove from the beginning
		add_to_queue(process_one);
		remove_from_queue();
		// the current process is now what is first in the queue
		return process_one->val->sp;
	}
			
}

