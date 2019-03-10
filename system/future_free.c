#include <xinu.h>
#include <stddef.h>
#include <stdlib.h>
#include <future.h> 

/* sets state of future to FREE */
syscall future_free(future *f){

  	return freemem((char*)f, sizeof(f));

}
