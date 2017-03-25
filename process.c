#include "3140_concur.h"
#include <stdlib.h>
#include <fsl_device_registers.h>

/* current process is NULL before scheduler starts and after it end */

process_t * current_process = NULL;



/* Creates a new process that starts at function f, initial stack size n 
Returns 0 on success, -1 if error 
May require allocating memory for a process_t structure, use malloc() 
Should called process_stack_init() to allocate the stack 
Should allocate a process_t 
Should add the process to your scheduler's data structures */
int process_create (void (*f) (void), int n)
{
	
}



void process_start (void);



/* PCB process control block */
struct process_state {
	int var1;
	struct process_state* some_pointer;
};

/* updates process_t*/
unsigned int * process_select(unsigned int * cursp);

