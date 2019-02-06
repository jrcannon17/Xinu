/*Global variable for producer consumer*/
extern int n; /*this is just declaration*/

/* Declare required semaphores */

extern sid32 consumed, produced;

/*function Prototype*/
void consumer(int count);
void producer(int count);
