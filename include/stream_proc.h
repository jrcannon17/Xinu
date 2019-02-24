/*Global variable for producer consumer*/
//extern type q[10]; /*this is just declaration*/
// need to use init to define 
/* Declare required semaphores */


/*function Prototype*/
void consumer(int count,int i);
void producer(int count);
void stream_proc(int nargs, char *args[]) ;

struct stream_data{
	int st;	// stream ID 
	int ts;	// timestamp
	int v;	// value
};

extern int num_streams;
extern int work_queue ;
extern int time_window ;
extern int output_time ;

/*Global variable for producer consumer*/
extern struct stream_data shared_queue[][500]; /*this is just declaration*/
extern sid32 consumed, produced;
extern sid32 mutex;

/*global pointer for tscdf */
extern struct tscdf *tscdf_pointer;

/*function Prototype*/
void consumer(int count, int i);
void producer(int count);
void tscdf_run(int count);
