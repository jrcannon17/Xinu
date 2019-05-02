/*Filename: apps/stream_proc.c for emacs gdb arm-eabi-gdb
Created by: Joshua Cannon
Created on: 20190207
Last Modified by: Joshua Cannon
Last Modified on: 20190425 * adding stream_proc_future
20190223 * TSDEF sym link broken or does not connect***FIXED
20190223 * working with Colin's ideas with mine with linked list. INCOMPLETE
20190214 * adding steam procs h file
20190131 * now includes semaphore protocol to fix async problem 
 */

#include <xinu.h>
#include <stdlib.h>
#include <stddef.h>
#include <stream_proc.h>
#include <future.h>
#include "stream_input.h"
#include "tscdf.h"

// input data as a global variable
const char *stream_input[];

// input parameters from command shell
int32 num_streams, work_queue, time_window, output_time;

struct data* read_queue(struct sem_stream* details) {

  // return data to tscdf logic
  	struct data* currData = details->head;
  	if (details->head != NULL) {

    	//printf("Data from linked list: id: %d,  value: %d \n", details->stream_id, details->head->value);

    		if (details->tail == details->head) {
      			details->tail = details->head  = NULL;
    		}
    // move head forward
    	details->head = details->head->next;
  	}

  return currData;
}

void sem_consumer(int32 out_time, struct sem_stream *obj) {
    	printf("Started consumer stream id = %d \n", obj->stream_id);
  	struct tscdf * tc = tscdf_init(time_window);
  	int32 count = 0,i;

    	while(1){
      		wait(obj->sem1_id);
      	//printf("Consuming in  id = %d , count %d  \n", obj->stream_id,count);
      		struct data* currData = read_queue(obj);
      		printf("currData value %d\n", currData->value);
      		tscdf_update(tc, currData->time, currData->value);
      		signal(obj->sem2_id);
      		count++;

      // check if quartiles are ready to be called
      		if (count == time_window){
        		int32 * query = tscdf_quartiles(tc);

        		if(query == NULL) {
            			printf("tscdf_quartiles is NULL\n");
           			 break;
        		}

        		printf("\n**********************Printing tscdf_quartiles*******************\n");

        		for(i=0; i < time_window; i++) {
           			printf("%d ", query[i]);
        		}
        		printf("\n************************SUMMARY**********************************\n");

        		freemem((char *)query, (time_window*sizeof(int32)));
        		count = 0;
      		}
    	}
}

void sem_producer(struct data* node, struct sem_stream *details){
  	wait(details->sem2_id);
  	printf("producing id: %d, data: %d \n",details->stream_id, node->value);
  	write_queue(node, details);
  	signal(details->sem1_id);
}

void write_queue(struct data* node, struct sem_stream* details) {
	if (details->head == NULL) {
    		details->head = node;
  	} 
	else {
    		details->tail->next = node;
  	}
  	details->tail = node;
  	node->next = NULL;
}

// stream processing using semaphores
int stream_proc( int32 nargs, char* args[]) {
	/* Parse arguments out of flags */
	/* if not even # args, print error and exit */

    	char usage[] = "Usage: -s num_streams -w work_queue -t time_window -o output_time\n", c;
    	char* ch;
    	int32 i;

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
    	}

    // create lookup queue to keep track of streams
struct sem_stream lookup[num_streams+2];
    	for (i=0; i< num_streams; i++) {
        	lookup[i].head = NULL;
    		lookup[i].tail = NULL;
    		lookup[i].stream_id = i;
    		lookup[i].sem1_id = semcreate(0);
    		lookup[i].sem2_id = semcreate(work_queue);
        	resume (create((void *) sem_consumer, 4096, 5, "sem_consumer", 2, output_time, &lookup[i]));
    	}

    // get data from stream_input
    	
	i = 0;
    	int32 st;
    	int total_data = sizeof(stream_input)/sizeof(stream_input[0]);
    	char* a;
    	// create the data object and add it to the respective queue based on the stream id.
    	while(i < total_data) {
      		struct data* node = (struct data *)  getmem (sizeof(struct data));
      		a = (char *)stream_input[i];
      		st = atoi(a);
      		while (*a++ != '\t');
      		node->time = atoi(a);
      		while (*a++ != '\t');
      		node->value = atoi(a);
      		node->next = NULL;

      		// add to queue
      		if (st < num_streams) {
        		sem_producer(node, &lookup[st]);
      		}
      		i++;
    	}

	// processing is done
    	sleep(4);
    	return 0;
}

void fut_consumer(int32 out_time, struct fut_stream *obj) {
	printf("Started consumer stream id = %d \n", obj->stream_id);
  	struct tscdf * tc = tscdf_init(time_window);
	int32 queue_count = 0;
  	int32 count = 0;

	struct pair* currData = (struct pair *)  getmem (sizeof(struct pair));

	while(1){
		
      		//printf("Consuming in  id = %d , count %d  \n", obj->stream_id,count);
		if(queue_count == work_queue) {
			break;
		}
		future_get(obj->fut_queue[queue_count], currData);
		kprintf("getting future value: %d    %d\n", currData->time, currData->value);


      		tscdf_update(tc, currData->time, currData->value);
      		count++;
		queue_count++;

      // check if quartiles are ready to be called
      		if (count == time_window){
        		int32 * query = tscdf_quartiles(tc);

        		if(query == NULL) {
            			kprintf("tscdf_quartiles is NULL\n");
				break;
        		}

        		printf("\n**********************Printing tscdf_quartiles*******************\n");

        		for(int i=0; i < 5; i++) {
           			kprintf("%d ", query[i]);
        		}
        		kprintf("\n************************SUMMARY**********************************\n");

        		freemem((char *)query, (6*sizeof(int32)));
        		count = 0;
      		}
    	}

    	return OK;
}

void fut_producer(struct pair* node, struct fut_stream* details){
		int i = 0;
		while(future_set(details->fut_queue[i], node) == SYSERR) {
			i++;
		}
			kprintf("Produced %d    %d\n", node->time, node->value);
  			//future_set(details->fut_queue[i], node);
  	return OK;
}

// stream processing using futures
int stream_proc_future( int32 nargs, char* args[]) {
	/* Parse arguments out of flags */
	/* if not even # args, print error and exit */

    	char usage[] = "Usage: -s num_streams -w work_queue -t time_window -o output_time\n", c;
    	char* ch;
    	int32 i;

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
    	}

	 // create lookup queue to keep track of streams
	struct fut_stream lookup[num_streams];
    	for (i=0; i< num_streams; i++) {
    		lookup[i].stream_id = i;

		// declaring future array
		lookup[i].fut_queue = getmem((sizeof(struct pair)) * work_queue);
		for(int j = 0; j < work_queue; j++) {
			// sizeof(int32) + sizeof(int32) = 4 + 4 = 8
			lookup[i].fut_queue[j] = future_alloc(FUTURE_EXCLUSIVE, sizeof(struct pair));
		}
		

        	resume (create((void *) fut_consumer, 4096, 5, "fut_consumer", 2, output_time, &lookup[i]));
    	}

	 // get data from stream_input
    	
	i = 0;
    	int32 st;
    	int total_data = sizeof(stream_input)/sizeof(stream_input[0]);
    	char* a;
    	// create the data object and add it to the respective queue based on the stream id.
    	while(i < total_data) {
      		struct pair* node = (struct pair *)  getmem (sizeof(struct pair));
      		a = (char *)stream_input[i];
      		st = atoi(a);
      		while (*a++ != '\t');
      		node->time = atoi(a);
      		while (*a++ != '\t');
      		node->value = atoi(a);

      		// add to queue
      		if (st < num_streams) {
        		fut_producer(node, &lookup[st]);
      		}
      		i++;
    	}

	// proccessing is done
	for(i = 0; i < num_streams; i++) {
		for(int j = 0; j < work_queue; j++) {
			future_free(lookup[i].fut_queue[j]);
		}
	}
	sleep(4);
	return 0;
}
