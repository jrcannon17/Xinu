/*Global variable for producer consumer*/
//extern type q[10]; /*this is just declaration*/
// need to use init to define 
/* Declare required semaphores */


typedef struct queue_lookup {
  int32 stream_id;
  sid32 semphr1_id;
  sid32 semphr2_id;
  struct data_element* stream;
  struct data_element* stream_tail;
  }ql;
 

/*function Prototype*/
void data_consumer(int out_time,ql *obj);
void data_producer(struct data_element * node, ql *details);
void stream_proc(int nargs, char *args[]) ;

/*struct data_element{ 
	int ts;	// timestamp
	int v;	// value
};
*/
extern int num_streams;
extern int work_queue ;
extern int time_window ;
extern int output_time ;

/*Global variable for producer consumer*/
extern sid32 mutex;

/*global pointer for tscdf */
extern struct tscdf *tscdf_pointer;
 
/*function Prototype*/
void tscdf_run(int count);
