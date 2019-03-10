#include <xinu.h>
#include <stddef.h>
#include <stdlib.h>
#include <future.h> 

future* future_alloc(int future_flags, uint size){
	
	future *f;

	f=(future *)getmem(sizeof(future));
	
	if(f == SYSERR){
		return SYSERR;
	}

	f->state = FUTURE_EMPTY;
	f->flags = future_flags;

	return f;
}
