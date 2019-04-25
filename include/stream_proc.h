#include <future.h>

// Structure to hold the data and a  pointer to the next structure in the linked list
struct data {
	int32 time, value;
	struct data* next;
};

// Structure for future implementation
struct pair {
	int32 time;
	int32 value;
};

// Structure to hold the information about the semaphores and streams
struct sem_stream {
// stream id , key for the lookup
  	int32 stream_id;
  // Producer semaphore
  	sid32 sem1_id;
  // Consumer semaphore
  	sid32 sem2_id;

  // head pointer to the stream linked list where consumer reads.
  	struct data* head;

  // tail pointer to the linked list where producer adds.
  	struct data* tail;
};

struct fut_stream {
	int32 stream_id;
	// initialized to work_queue length
	future** fut_queue;
	// struct pair key;
};

struct data* read_queue(struct sem_stream* details);

void write_queue(struct data* node, struct sem_stream* details);

void sem_consumer(int32 out_time, struct sem_stream* obj);

void sem_producer(struct data* node, struct sem_stream* details);

void fut_consumer(int32 out_time, struct fut_stream* obj);

void fut_producer(struct pair* node, struct fut_stream* details);

int stream_proc( int32 nargs, char* args[]);

int stream_proc_future( int32 nargs, char* args[]);
