/*Filename: future_get.c *
 *Part of: Futures program assignment *
 *Created by: Joshua Cannon *
 *Created on: 03/01/2019 *
 *Last Modified by: Joshua Cannon *
 *Last Modified on: 03/11/2019
 *Notes: There are several ways to do this. I've seen at least three different attempts, but me being me, I stick to what I know best (call suspend). The other way is to call upon procent and use the PR_WAIT definition on the pid once it has been captured. Another way is to make up a new wait, called fwait and just call fwait(future pointer struct) on the entire thing. Each have their advantages and mine is probably the lowest, but it works so sue me. Out 
 */

#include <xinu.h>
#include <stddef.h>
#include <stdlib.h>
#include <future.h> 

//--------------------------//
//-------FIXED-------------//

//issue 1 
/*when future_get is called on, the value is never copied into the arguements value. Should happen after process is suspended*/

//take future value copy 
syscall future_get(future *f, char *value) {
 

  intmask mask; 
  struct procent *prptr;

mask=disable();
	if (f->state == FUTURE_FULL){
	  memcpy(value, f->value, f->size);
	  //*value = f->value; 
		restore(mask);
		return OK; 
	} 	
 if (f->flags == FUTURE_EXCLUSIVE) {
      	if(f->state == FUTURE_EMPTY){

          f->pid = getpid();
	  f->state = FUTURE_WAITING;
	  f->pid=currpid;
	  suspend(currpid);
	  memcpy(value, f->value, f->size);
	  // *value = f->value;
	  // resched();
	}
 else if(f->state == FUTURE_FULL) {
   memcpy(value, f->value, f->size);
   f->state = FUTURE_EMPTY;
 }
 else{
	  	restore(mask);
	  	return SYSERR;
}
 }
else if (f->flags == FUTURE_SHARED) {
      if (f->state == FUTURE_EMPTY) {
          f->pid = getpid();
          f->state = FUTURE_WAITING;

          add_to_queue(f, 1);
          suspend(f->pid);
          // value = f->value;
          memcpy(value, f->value, f->size);
      }
   
  else if(f->state == FUTURE_FULL) {
         memcpy(value, f->value, f->size);
          //f->state = FUTURE_EMPTY;
      } else if (f->state == FUTURE_WAITING) {
        add_to_queue(f, 1);
        f->pid = getpid();
        suspend(f->pid);
        strncpy(value, f->value, f->size);
      }
      else {
          restore(mask);
          return SYSERR;
      }
    } else if (f->flags == FUTURE_QUEUE) {
      if (f->state == FUTURE_EMPTY) {
          f->pid = getpid();
          f->state = FUTURE_WAITING;
          add_to_queue(f,1);
          suspend(f->pid);
          memcpy(value, f->value, f->size);
      }
      else if(f->state == FUTURE_FULL) {
          strncpy(value, f->value, f->size);
          f->state = FUTURE_EMPTY;
         
      } else if (f->state == FUTURE_WAITING) {
        if (f->set_queue != NULL) {
          pid32 newpid = get_process(f,0);
          struct procent* prptr = &proctab[newpid];
          prptr->prprio = prptr->prprio+1;

          resume(newpid);
          strncpy(value, f->value, f->size);
        } else {
          add_to_queue(f, 1);
          suspend(getpid());
          memcpy(value, f->value, f->size);
        }
      }
      else {
          //printf("fvalue in  else get %d\n", f->value);
          restore(mask);
          return SYSERR;
      }
 }else{
  restore(mask);
  return SYSERR;
 }
restore(mask);
return OK;
}
