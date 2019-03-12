#include <xinu.h>
#include <stddef.h>
#include <stdlib.h>
#include <future.h> 

/* if future does not have value set */
syscall future_set(future *f, char *value) {
	
	intmask	mask;
	mask=disable();
	if(f->state == FUTURE_WAITING){
	  f->value = *value;
	  ready(f->pid);
	  f->state = FUTURE_FULL;
	  resched();
	  restore(mask);
	  return OK;
	}

	if (f->state == FUTURE_EMPTY){
	  f->value = *value;
		f->state = FUTURE_FULL; 
		restore(mask);
		return OK; 
	}	

	
	restore(mask);
       	return SYSERR;
}
