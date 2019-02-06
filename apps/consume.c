/*Filename: apps/consume.c
Created by: Joshua Cannon
Created on: 20190118
Last Modified by: Joshua Cannon
Last Modified on: 
20190126 * wait, signal, now includes semaphore protocol to fix async problem
20190201 * when make, for loop is shown as having no effect. No change made 
20190206 * modified syntax to look cleaner when ran with produce.c
 */
#include <xinu.h>
#include <prodcons.h>

void consumer(int count) { 

  int i = 0; //set up iterator 
  int c; // set up consumed variable 
  
for (i; i <= count; i++)
    { 
      // sleep(consumed);
      wait(produced);

      c = n; //consumed becomes the global variable 
 // reads the value of the global variable 'n', 'count' times.
      printf("Consumed: %d\n", c); // print consumed value e.g. consumed : 8
      signal(consumed);
    } 
// not sure if this is needed or not?
// return; 
}
