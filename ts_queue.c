#include "ts_queue.h"
#include "debug.h"
#include <stdlib.h>
#include <string.h>


void ts_queue_init( ts_queue * src )
{
	src->lock = malloc( sizeof( pthread_mutex_t ) );
	pthread_mutex_init( src->lock, NULL );
}

void * ts_queue_pop( ts_queue * src )
{
	pthread_mutex_lock( src->lock );
	if( src->first == NULL )
	{
		pthread_mutex_unlock( src->lock );
		return NULL;
	}
	
	ts_queue_node * c = src->first;
	if( c->next != NULL )
	{
		src->first = c->next;
		src->first->prev = NULL;
	}
	else
		src->first = NULL;
	void * d = c->data;
	//free( c );
	pthread_mutex_unlock( src->lock );
	return d;
}

void ts_queue_push( ts_queue * src0, void * src1, size_t src2 )
{
	pthread_mutex_lock( src0->lock );
	ts_queue_node * n = calloc( 1, sizeof( ts_queue_node ) );
	n->data = malloc( src2 );
	memcpy( n->data, src1, src2 );
	if( src0->last != NULL )
	{
		src0->last->next = n;
		n->prev = src0->last;
	}
	if( src0->first == NULL )
		src0->first = n;
	src0->last = n;
	pthread_mutex_unlock( src0->lock );
}
