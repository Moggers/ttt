#ifndef TS_QUEUE_IG
#define TS_QUEUE_IG
#include <pthread.h>
#define TS_QUEUE_FINISHED_INIT 1337

typedef struct ts_queue_node
{
	void * data;
	struct ts_queue_node * next;
	struct ts_queue_node * prev;
	
} ts_queue_node;

typedef struct ts_queue
{
	int init;
	struct ts_queue_node * first;
	struct ts_queue_node * last;
	pthread_mutex_t * lock;
} ts_queue;

void * ts_queue_pop( ts_queue * src );
void ts_queue_push( ts_queue * src0, void * src1, size_t src2  );
void ts_queue_init( ts_queue * src );
#endif
