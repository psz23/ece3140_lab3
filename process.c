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

struct process_queue *process_one = NULL;


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
	if (process_one == NULL) {
		process_one = new_process;
		new_process->next = NULL;
	} 
	else {
		struct process_queue *tmp = process_one;
		while (tmp->next != NULL) {
			tmp = tmp->next;
		}
		tmp->val->next_process = new_process->val;
		tmp->next = new_process;
		new_process->next = NULL;
	}
	return 0;
};

void process_start (void)
{
	PIT->MCR = 0;
	PIT->CHANNEL[0].LDVAL = 0x1E8480;
	NVIC_EnableIRQ(PIT0_IRQn);
	current_process = process_one->val;
	process_begin();
}

/* process_t is NULL until process_start is called
also NULL when a process terminates */

/* Use process_stack_free when a process terminates */


/* updates process_t*/
unsigned int * process_select(unsigned int * cursp)
{
	if (cursp == NULL) { //if cursp is null, process is finished or nothing was running
		if (current_process == NULL) { //nothing was running
			if (process_one == NULL) return NULL;
			else{
				current_process = process_one->val;
				return current_process->sp;
			}
		}
		else { //just finished process
			if (curr
			
}

