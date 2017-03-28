/* What if there are many short processes and one long process 
We expect that after p1 and p2 get some time to execute, p3 will 
execute but will not finish. Until p1 and p2 finish, p3 will ocassionally 
execute and then be interrupted by the timer to allow p1 and p2 to execute
until done. Say the timer allows the LEDs to toggle twice before the timer
runs out. p1 and p2 toggle 4x, p3 20x. Each new line signals a timeout.
Execution would look like this:
Initial queue: p1 p2 p3
Execution:
p1
p2
p3
p1
p2 <== here p1 and p2 have both finished their processes
p3 <== after p3 executes a second time, it must toggle 16 more times
p3 <== 14 more times
p3
... <== p3 executes until it finishes, so it requires 8 timeouts after the second p3 timeout
p3
*/

#include "3140_concur.h"
#include "utils.h"

/* Red toggle x4; short process #1 */
void p1 (void)
{
	int i;
	for (i=0; i < 4; i++) {
		delay ();
    LEDRed_Toggle();
	}
}

/* Blue toggle x4; short process #2 */
void p2 (void)
{
	int i;
	for (i=0; i < 4; i++) {
		delay ();
    LEDBlue_Toggle();
	}
}

/* Green toggle x20
This is the super long process */
void p3 (void)
{
	int i;
	for (i=0; i < 20; i++) {
		delay ();
		LEDGreen_Toggle();
	}
}

/* This test makes sure that round robin scheduling is correct.
Since the green toggle, p3, requires much more time to complete, 
it should be interrupt and kicked to the back of the queue to allow
the red and blue toggles, p1 and p2, to finish.
Green LED on at the end signals the processes all finished. */
int main (void)
 {
 LED_Initialize();
 
 if (process_create (p1,20) < 0) {
 	return -1;
 }

 if (process_create (p2,20) < 0) {
 	return -1;
 }
 
 if (process_create (p3,20) < 0) {
	return -1;
 }
 
 process_start ();
 
 LEDGreen_On();
 
 while (1) ;

 return 0;
}
