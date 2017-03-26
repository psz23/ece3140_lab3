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
	return 0;
};



void process_start (void)
{
	NVIC_EnableIRQ(PIT0_IRQn);
	
	process_begin();
}

/* process_t is NULL until process_start is called
also NULL when a process terminates */

/* Use process_stack_free when a process terminates */


/* updates process_t*/
unsigned int * process_select(unsigned int * cursp){
	// help i'm die
}

