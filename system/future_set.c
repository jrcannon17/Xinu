#include <xinu.h>
#include <stddef.h>
#include <stdlib.h>
#include <future.h> 

/* if future does not have value set */
syscall future_set(future *f, char *value) {
	
	intmask	mask;
	mask=disable();
	
    if (f->flags == FUTURE_EXCLUSIVE) {
      if (f->state == FUTURE_EMPTY) {
          //f->value = value;
          memcpy(f->value, value, f->size);
          f->state = FUTURE_FULL;
      }
	else if(f->state == FUTURE_WAITING){
	   memcpy(f->value, value, f->size);
	   //f->value = *value;
	   //ready(f->pid);
	  f->state = FUTURE_FULL;
	  //resched();
	  resume(f->pid); 
	}
	  else{
	    restore(mask);
	  return OK;
	}
	}
	else if (f->flags == FUTURE_SHARED) {
	if (f->state == FUTURE_EMPTY){
	  memcpy(f->value, value, f->size);
//f->value = *value;
		f->state = FUTURE_FULL; 
		
	}
 else if (f->state == FUTURE_WAITING) {
          memcpy(f->value, value, f->size);
          //printf("fvalue is :%d WAITING values is :%d: ", *f->value, *value);
          f->state = FUTURE_FULL;
          pid32 p_id;
          while((p_id = get_process(f, 1)) > -1 ) {
            resume(p_id);
	  }
 }
 else{
   restore(mask);
   return SYSERR;
 	}	
	}
else if (f->flags == FUTURE_QUEUE) {
      if (f->state == FUTURE_EMPTY) {
          //f->value = value;
          add_to_queue(f, 0);
          f->state = FUTURE_WAITING;
          suspend(getpid());
          strncpy(f->value, value, f->size);
      }
      else if (f->state == FUTURE_WAITING) {
          if(f->get_queue != NULL) {
            strncpy(f->value, value, f->size);
            pid32 newpid = get_process(f,1);
            struct procent* prptr = &proctab[newpid];
            prptr->prprio = prptr->prprio+1;
            resume(newpid);
          } else {
            add_to_queue(f, 0);
            suspend(getpid());
            strncpy(f->value, value, f->size);
          }
      }
      else{	
	restore(mask);
       	return SYSERR;
}
 }
 else{
   printf("Error. Wrong mode");
      restore(mask);
      return SYSERR;
    }
    restore(mask);
    return OK;
}
