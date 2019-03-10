#include <xinu.h>
#include <stddef.h>
#include <stdlib.h>
#include <future.h> 

/* if future does not have value set */
syscall future_get(future *f, char *c) {
	
	intmask	mask;
	mask=disable();
	if (f->state == FUTURE_EMPTY){
	
		f->value = *c;
		f->state = FUTURE_EXCLUSIVE; 
		restore(mask);
		return OK; 
	} 	

	if(f->state == FUTURE_WAITING){
		f->value = *c;
		printf("Producer (PID : %d) signaling Consumer (PID : %d) \n", currpid, f->pid);
		f->state=FUTURE_EXCLUSIVE;
		restore(mask);
		return OK;
	}
	restore(mask);
	return SYSERR;
}
