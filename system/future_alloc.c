/*Filename: future_alloc.c *
 *Part of: Futures program assignment *
 *Created by: Joshua Cannon *
 *Created on: 03/01/2019 *
 *Last Modified by: Joshua Cannon *
 *Last Modified on: 03/11/2019
 *Notes: must getmem to create a future process. PID I found, is not needed: we do this in future_get so I'm leaving that alone for now, most likely forever. Make a pointer struct to put the values in. 
 */

#include <xinu.h>
#include <stddef.h>
#include <stdlib.h>
#include <future.h> 

future *f; // pointer to future structure

future* future_alloc(int future_flags, uint size){
	 

  f =(future *)getmem(sizeof(future));
	
	if(f == SYSERR){
		return SYSERR;
	}
   
	//	f->pid = getpid();
       	f->value = getmem(size);
	f->size = size;
	f->state = FUTURE_EMPTY;
	f->flags = future_flags;
	f->set_queue = NULL;
	f->get_queue = NULL;

	return f;
}
