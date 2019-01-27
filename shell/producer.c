/*Filename: Producer.c
Created by: Joshua Cannon
Created on: 20190122
Last Modified by: Joshua Cannon
Last Modified on: 20190126 * does not run; multiple definitions defined
 */
#include <xinu.h>
#include <prodcons.h>

void producer(int count) 
{  
  int i; // will set the iterator  as 0 in the for loop

  for(i = 0 ; i < count; i++)
    {

  // Iterates from 0 to count, setting
    // the value of the global variable 'n' each time     
      n = i; 
     
      printf("Produced: %d\n",n); //print produced value e.g. produced : 8

      if (n == count) // Let the user know when the count has reached the global variable
	  {
	    printf("Produce Stopped\n");
	  }
    }
     
  // return;
}
