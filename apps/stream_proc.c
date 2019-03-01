/*Filename: apps/stream_proc.c for emacs gdb arm-eabi-gdb
Created by: Joshua Cannon
Created on: 20190207
Last Modified by: Joshua Cannon
Last Modified on: 
20190223 * TSDEF sym link broken or does not connect***FIXED
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
#include <stddef.h>
#include "tscdf.h"


sid32 producer;
sid32 consumer; // semaphor definitions
sid32 mutex;

// struct stream_data shared_queue[100][500];

char usage[] = 
"Usage: -s num_streams -w work_queue -t time_window -o output_time \n";

// define usage as arguments

int num_streams = 10;
int work_queue =10;
int time_window = 25;
int output_time = 10;

// hard coded definitions of input value from picture example


// idea not valid anymore 
/*
struct stream 
{
  int r;
  int w;
  int m;
}
*/
/*
typedef struct queue_lookup {
  int32 stream_id;
  sid32 semphr1_id;
  sid32 semphr2_id;
  struct data_element* stream;
  struct data_element* stream_tail;
  }ql;
*/
struct data_element {
  
	int ts;	// timestamp
	int v;	// value
        struct data_element *next;
};

struct queue_lookup lookup[10];

// put in readable format: data element
void printqueue(struct queue_lookup* details){
  struct data_element * temp = details->stream;

  while (temp != NULL) {
    printf("Data Read: id: %d, value: %d\n",details->stream_id, temp->v);
    temp = temp->next;
  }
}


struct tscdf *typedef_pointer;


struct data_element * read_data_from_queue(struct queue_lookup* details) {
  struct data_element * currData = details->stream;
  if (details->stream != NULL) {


    if (details->stream_tail == details->stream) {
      details->stream_tail = details->stream  = NULL;
    }

    details->stream = details->stream->next;
  }

  return currData;
}
//begin consumer process 
//conflicting types
void data_consumer(int out_time,ql *obj) {
    printf("Started consumer stream id = %d \n", obj->stream_id);
  	struct tscdf * tc = tscdf_init(time_window);
  	int32 count = 0,i;
	
	while(1){
	  wait(obj->semphr1_id);
	  printf("Consuming in id = %d, count %d\n", obj->stream_id,count);
	  struct data_element * currData = read_data_from_queue(obj);
	  printf("currData value %d\n", currData->v);
	  tscdf_update(tc, currData->ts, currData->v);
	  signal(obj->semphr2_id);
	  count++;
      // check if count is equal to output_time for quartile data struct
	  if (count == time_window){
	    int32 * qarray = tscdf_quartiles(tc);
	    if(qarray ==NULL) {
	      printf("tscdf_quartiles returned NULL\n");
	      break;
	}
	    printf("Printing tscdf_quartiles\n");

  for(i=0; i < time_window; i++) {
           printf("%d ", qarray[i]);
        }
        printf("\n");
       
	// freememory space from queue
	freemem((char *)qarray, (time_window*sizeof(int32)));
	count = 0;
      }
	}
}

void add_data_to_queue(struct data_element * node, ql* details){
 printf("In adding to queue, node val: %d\n", node->v);
  if (details->stream == NULL) {
    printf("Head was null\n");
    details->stream = node;
    details->stream_tail = node;
  } else {
   
    details->stream_tail->next = node;
    details->stream_tail = node;
   
 }

  node->next = NULL;
}

//producer process 

void data_producer(struct data_element * node, ql *details){
  wait(details->semphr2_id);
  printf("producing id: %d, data_element : %d ",details->stream_id, node->v);
  add_data_to_queue(node, details);
  signal(details->semphr1_id);
}

int stream_procs(int32 nargs, char *args[])
{
 char usage[] = "Usage: -s num_streams -w work_queue -t time_window -o output_time\n", c;

 int count; 
  
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


	int32 i; 
	char* ch;
	//int timer;

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
 
// create num_streams q's of work_q size 


   for (i=0; i< num_streams; i++) {
      lookup[i].stream = NULL;
    	lookup[i].stream_tail = NULL;
    	lookup[i].stream_id = i;
    	lookup[i].semphr1_id = semcreate(0);
    	lookup[i].semphr2_id = semcreate(work_queue);
    	printf(" adding row for id: %d",i);
      resume (create((void *) data_consumer, 4096, 5, "data_consumer", 2, output_time, &lookup[i]));
    }

   /*
 sstreams = (struct stream *)
   getmem(num_streams * sizeof(struct stream));
 if ((int32)sstreams == SYSERR) {
   printf("getmem failed\n");return(-1);
 }
  */

 //initialize and start process 

   /*
 for(i=0 < num_streams; i++) {
   char id_str[16];
   sstreams[i].r = semcreate(0);
   sstreams[i].w = semcreate(work_queue);
   sstreams[i].m =semcreate(1);
   sstreams[i].head =0;
   sstreams[i].tail =0;
   sstreams[i].queue = (de *)getmem(work_queue * sizeof(de));
   resume(create(stream_consumer, 1024, 20, &id_str[0],3, i, &sstreams[i]));
 }
   */

 // begin reading streaming data 

   i = 0;

   // for(i=0; i < n_input; i++) {
   int32 st,time_stamp, value;
   int total_data = sizeof(stream_input)/sizeof(stream_input[0]);
   struct data_element *de;
   char *a;

printf("total_data : %d\n", total_data);
    while(i < total_data){
      a = (char *)stream_input[i];
      st = atoi(a);
      printf("for in while id: %d\n", st);
      while (*a++ != '\t');
      time_stamp = atoi(a);
      while (*a++ != '\t');
      value = atoi(a);
    }
   /*
 producer = semcreate(0);
 consumer = semcreate(work_queue);
 mutex = semcreate(1);
   */

struct data_element * node = (struct data_element  *)  getmem (sizeof(struct data_element ));
      node->ts = time_stamp;
      node->v = value;
      node->next = NULL;
      //add to queue 
      if (st < num_streams) {
	data_producer(node, &lookup[st]);
      }
      i++;
    }
    sleep(4);
    return 0;
 }


/*
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
*/
