/* What if there is only one call to process_create()
We expect that when process_create() is only called once, it only adds
that process that was created to the queue. p2 does not execute because
it was not added to the queue, therefore only the red LED toggles */

#include "3140_concur.h"
#include "utils.h"

/* Red toggle x10 */
void p1 (void)
{
	int i;
	for (i=0; i < 10; i++) {
		delay ();
    LEDRed_Toggle();
	}
}

/* Blue toggle x10 */
void p2 (void)
{
	int i;
	for (i=0; i < 10; i++) {
		delay ();
    LEDBlue_Toggle();
	}
}

/* Create process for p1, which adds to queue.
This ensures that only processes created with process_create are
executed.
Green LED on at the end signals the processes all finished. */
int main (void)
 {
 LED_Initialize();
 
 if (process_create (p1,20) < 0) {
 	return -1;
 }

 process_start ();
 
 LEDGreen_On();
 
 while (1) ;

 return 0;
}
