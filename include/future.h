/*Filename: future.h *
 *Part of: Futures program assignment *
 *Created by: Joshua Cannon *
 *Created on: 03/01/2019 *
 *Last Modified by: Joshua Cannon *
 *Last Modified on: 03/11/2019
 *Notes: define NFUTURES in config file (most likely, the armsx86 version). Refer to NSEM for more information on how this can be done. One thing I don't get is why take char* instead of int*. The code nevertheless, should work for both however.  
 */
#include <xinu.h>
#include <stddef.h>
#include <stdlib.h>

#ifndef _FUTURE_H_
#define _FUTURE_H_

// may need struct or array called FUTURE_VALID that can determine all valid states of future
 
/* define states */
#define FUTURE_FREE	  0
#define FUTURE_EMPTY 	  1
#define FUTURE_FULL 	  3
#define FUTURE_WAITING  5

/* modes of operation for future*/
#define FUTURE_EXCLUSIVE  0	

typedef struct futent

 {
  uint32 state;
  uint16 flags;
  uint32 size;
  char *value; /* alloc must memget size (in chars) space */
  pid32 pid; /* for single waiter case */
} future;

extern struct futent futtab[];

/* Interface for system call */
future* future_alloc(int future_flags, uint size);
syscall future_free(future*);
syscall future_get(future*, char *);
syscall future_set(future*, char *);
//future_test(int nargs, char *args[]);
 
#endif /* _FUTURE_H_ */
