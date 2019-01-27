/*Filename: consumer.c
Created by: Joshua Cannon
Created on: 20190122
Last Modified by: Joshua Cannon
Last Modified on: 20190126 * does not run; multiple definitions defined
 */

#include <xinu.h>
#include <prodcons.h>

void consumer(int count) { 

  int i = 0; //set up iterator 
  int c; // set up consumed variable 
  
for (i; i <= count; i++)
    { 
      c = n; //consumed becomes the global variable 
 // reads the value of the global variable 'n', 'count' times.
      printf("Consumed : %d\n", c); // print consumed value e.g. consumed : 8
    } 
// not sure if this is needed or not?
// return; 
}
