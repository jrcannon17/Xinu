/*Filename: apps/stream_proc.c for emacs gdb arm-eabi-gdb
Created by: Joshua Cannon
Created on: 20190207
Last Modified by: Joshua Cannon
Last Modified on: 
20190223 * TSDEF sym link broken or does not connect
20190223 * working with Colin's ideas with mine with linked list. INCOMPLETE
20190214 * adding steam procs h file
20190131 * now includes semaphore protocol to fix async problem 
 */

#include <xinu.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stream_input.h>
#include <stream_proc.h>
#include <tscdf.h>


sid32 producer, consumer; // semaphor definitions
sid32* array_of_semaphors;
sid32 mutex;

struct stream_data shared_queue[100][500];

char usage[] = 
"Usage: -s num_streams -w work_queue -t time_window -o output_time \n";

// define usage as arguments

	int num_streams;
	int work_queue;
    int time_window;
	int output_time;

	//make q struct; link list
typedef struct 

{ 
  int time_stamp;
  int value;
  NODE *head;
  NODE *tail;
  } q;

struct NODE {
  int number;
  struct NODE *next;
}
// make q deque, enque, new_q 

NODE * Dequeue(q *pq) {
    /*the queue is empty or bad param*/
    NODE *item;
    if (isEmpty(pQueue))
        return NULL;
    item = q->head;
    pq->head = (pq->head)->prev;
    pq->size--;
	
    return item;
}

int Enqueue(Queue *pQueue, NODE *item) {
    /* Bad parameter */
    if ((pq == NULL) || (item == NULL)) {
        return FALSE;
    }
  
    if (pq->size >= pq->limit) {
        return FALSE;
    }
    /*the queue is empty*/
    item->prev = NULL;
}
struct tscdf *typedef_pointer;

void stream_procs(int nargs, char *args[])

{
    // catch empty strings
  if (nargs == 2)
    {
      printf("No argument present\n");
    }
count = (atoi)(usage[1]);

	// if command line count 0 let user know
 if(count == 0)
   {
     printf("Nothing to produce.\n");
     return 0;
   }

   /* Parse arguments out of flags */
/* if not even # args, print error and exit */

// hard coded definitions of input value from picture example

	 num_streams = 10;
	 work_queue = 10;
     time_window = 25;
	 output_time = 10;

	int i;
	char c; 
	char *ch;

if (!(nargs % 2)) {
  printf("%s", usage);
  return(-1);
}
else {
  i = nargs - 1;
  while (i > 0) {
    ch = args[i-1];
    c = *(++ch);
    
    switch(c) {
    case 's':
      num_streams = atoi(args[i]);
      break;

    case 'w':
      work_queue = atoi(args[i]);
      break;

    case 't':
      time_window = atoi(args[i]);
      break;
      
    case 'o':
      output_time = atoi(args[i]);
      break;

    default:
      printf("%s", usage);
      return(-1);
    }

    i -= 2;
  }
// put args into readable format
 printf("num_streams: %d\nwork_queue: %d\ntime_window: %d\noutput_time: %d\n", num_streams, work_queue, time_window, output_time);
 
 producer = semcreate(0);
 consumer = semcreate(work_queue);
 mutex = semcreate(1);
 
 printf("%d\n", sizeof(stream_input)/sizeof(stream_input[0]));
 
 int32 num_elements = sizeof(stream_input)/sizeof(stream_input[0]);

	resume( create(producer, 1024, 20, "producer", 1, num_elements));

	/* from Colin: "now create num_streams threads for the consumers" */
	for(int i = 0; i < num_streams; i++) {
			resume( create(consumer, 1024, 20, "consumer", 1, num_elements, i));
	}
	/*
		/* start the tscdf function */
	tscdf_pointer = tscdf_init(time_window);
	
void consumer(int count, int st_id) {

	/* wait to recieve stream_data from producer */
	/* only consume stream_data if st matches this stream's ID */
	/* i is the stream_id */
	/* print the data FCFS from the queue */
	/* when queue is empty, wait */
	for(int k = 0; k < count; k++) {
		for(int y = 0; y < work_queue; y++) {
		/* print the timestamp-value pairs for each element in the queue */
			wait(producer);
			wait(mutex);
			
			struct stream_data current = shared_queue[st_id][y];
			/* only consume matching nonzero elements */
			if(st_id == current.st && current.v != 0) {
				//printf("stream : %d\ttimestamp : %d\tvalue : %d\n", current.st, current.ts, current.v);
				tscdf_update(tscdf_pointer, current.ts, current.v);
			}
			signal(mutex);
			signal(consumer);
		
		}
	
	}
}

void producer(int count) {
	/* import stream_input and produce from the data there */
	/* pull elements from stream_input into global queue equal to work_queue arg */
	/* if queue is full, wait */
	/* once queue is empty, read data until queue is full */
	char  *a;
	for(int k = 0; k < count; k++) {
		for(int y = 0; y < work_queue; y++) {
			struct stream_data current;

			a = (char *)stream_input[y];
  			current.st = atoi(a);
  			while (*a++ != '\t');
  			current.ts = atoi(a);
  			while (*a++ != '\t');
  			current.v = atoi(a);

			wait(cons);
			wait(mutex);
			/* copy the input string into the shared queue for the given stream */
			shared_queue[current.st][y] = current;
		
			timer++;
			
			if(timer == output_time ) {
				timer = 0;
				int32 *qarray = tscdf_quartiles(tscdf_pointer);
				if(qarray == NULL) {
   				 	kprintf("tscdf_quartiles returned NULL\n");
				}

				for(int i=0; i < 5; i++) {
   					kprintf("%d ", qarray[i]);
				}
					kprintf("\n");
			}
			
			signal(mutex);
			signal(producer);
		}
	
	}
		
}

void tscdf_run(int count) {
	for(int x = 0; x < count; x++) {
		if(timer >= output_time) {
			timer = 0;
			int32 *qarray = tscdf_quartiles(tscdf_pointer);
			if(qarray == NULL) {
   				 kprintf("tscdf_quartiles returned NULL\n");
			}

			for(int i=0; i < 5; i++) {
   				kprintf("%d ", qarray[i]);
			}
				kprintf("\n");
		
		}
	}
}

