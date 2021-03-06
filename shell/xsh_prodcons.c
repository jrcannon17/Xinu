/*Filename: shell/xsh_prodcons.c
Created by: Joshua Cannon
Created on: 20190122
Last Modified by: Joshua Cannon
Last Modified on: 
20190131 * now includes semaphore protocol to fix async problem 
20190126 * readies consumer.c and producer.c in the queue
 */

#include <xinu.h>
#include <prodcons.h>
#include "shprototypes.h"
//#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int n;                 //Definition for global variable 'n'
sid32 consumed, produced; // Define producer, consumer semaphores

int isNumeric(const char *str) //we will need this to verify if the input is numeric or not
{
  while(*str != '\0') //stop loop when str is numeric
    {
      if(*str < '0' || *str > '9') //if string is in between 0 and 9
	return 0;
      str++;
    }
  return 1;
}

/*Now global variable n will be on Heap so it is accessible all the processes i.e. consume and produce*/

shellcmd xsh_prodcons(int nargs, char *args[])
{
  //Argument verifications and validations

  int count = 100;             //local varible to hold count
  
  //injection prevention measures 
  if(nargs > 2)
    {
      printf("Only one argument.\n",args[0]);
      return 1;
    }
 
  if (!isNumeric(args[1])) //this will still allow the user to put in a string, but we won't them to know it is wrong
    {
      printf("Must be a numeral.\n");
      return 0;
    }
  
  count = atoi(args[1]); // the string is converted into a numeric
    
  // Define semaphores 

  produced = semcreate(0);
  consumed = semcreate(1); 
  
  //Create the process producer and consumer and put them in ready queue.
  //Look at the definations of function create and resume in the system folder for reference.      
  resume( create(producer, 1024, 20, "producer", 1, count));
  resume( create(consumer, 1024, 20, "consumer", 1, count));
  return (0);
}
