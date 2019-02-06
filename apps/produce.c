/*Filename: apps/produce.c
Created by: Joshua Cannon
Created on: 20190118
Last Modified by: Joshua Cannon
Last Modified on: 
20190126 * wait, signal, now includes semaphore protocol to fix async problem 
 */
#include <xinu.h>
#include <prodcons.h>

void producer(int count) 
{  
  int i; // will set the iterator  as 0 in the for loop

  for(i = 0 ; i < count; i++) // Iterates from 0 to count, setting value global variable 'n' each time
    {
      wait(consumed);
  

      n = i; 
     
      printf("Produced: %d\n",n); //print produced value e.g. produced : 8
      signal(produced);
      
      /* if (n == count) // Let the user know when the count has reached the global variable
	  {
	    printf("Produce Stopped\n");
      */  
  }
  
// return;

}
