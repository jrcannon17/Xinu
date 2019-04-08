/*Filename: future_free.c *
 *Part of: Futures program assignment *
 *Created by: Joshua Cannon *
 *Created on: 03/01/2019 *
 *Last Modified by: Joshua Cannon *
 *Last Modified on: 03/11/2019
 *Notes: When this is called, we free the memory of: a) the args, and b)the size of the struct. Do NOT overthink this procedure as this was all it was made to do.
 */
#include <xinu.h>
#include <stddef.h>
#include <stdlib.h>
#include <future.h> 

/* sets state of future to FREE */
syscall future_free(future *f){

    freemem(f->value, f->size);
    free_queue(f->set_queue);
    free_queue(f->get_queue);
    return freemem((char*)f, sizeof(f));

}
