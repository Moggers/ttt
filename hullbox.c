#include "hullbox.h"
#include <stdlib.h>
#include "stack.h"
#include "render.h"
#include <string.h>

hullbox_list hullbox_set;

int gl_stack_size;
stack open_slot;

int hullbox_create( void )
{
	int retid = stack_pop( &open_slot );
	stack_push( &open_slot, retid + 1 );
	
	render_inc_box_count();
	hullbox_set[retid] = calloc( 1, sizeof( hullbox ) );
	hullbox_set[retid]->gl_stack_position = gl_stack_size++;
	hullbox_set_pos( retid, &((vec3f){ 0.0f, 0.0f, 0.0f }) );
	hullbox_set_ang( retid, &((vec3f){ 0.0, 0.0, 0.0 }) );
	hullbox_set_scale( retid, &((vec3f){ 1.0, 1.0, 1.0 }) );
	hullbox_set_color( retid, &((vec4f){ 1.0, 1.0, 1.0, 1.0 }) );

	return retid;
}
void hullbox_delete( int id )
{
	stack_push( &open_slot, id );
	gl_stack_size--;
	render_dec_box_count();
	if( gl_stack_size == 0 ) return;
	int i;
	for( i = 0; i <= gl_stack_size; i++ )
	{
		if( hullbox_set[i]->gl_stack_position == (gl_stack_size) )
		{
			hullbox_set[i]->gl_stack_position = hullbox_set[id]->gl_stack_position;
			hullbox_set_pos( i, &hullbox_set[i]->pos );
			hullbox_set_ang( i, &hullbox_set[i]->ang );
			hullbox_set_scale( i, &hullbox_set[i]->scale );
			hullbox_set_color( i, &hullbox_set[i]->color );
		}
	}
}

int hullbox_set_pos( int id, vec3f * src )
{
	memcpy( &(hullbox_set[id]->pos), src, sizeof( vec3f ) );
	render_set_box_pos( hullbox_set[id]->gl_stack_position, src );
}
int hullbox_set_ang( int id, vec3f * src )
{
	memcpy( &(hullbox_set[id]->ang), src, sizeof( vec3f ) );
	render_set_box_ang( hullbox_set[id]->gl_stack_position, src );
}
int hullbox_set_scale( int id, vec3f * src )
{
	memcpy( &(hullbox_set[id]->scale), src, sizeof( vec3f ) );
	render_set_box_scale( hullbox_set[id]->gl_stack_position, src );
}
int hullbox_set_color( int id, vec4f * src )
{
	memcpy( &(hullbox_set[id]->color), src, sizeof( vec3f ) );
	render_set_box_color( hullbox_set[id]->gl_stack_position, src );
}

const vec3f * hullbox_get_pos( int id )
{
	return (const vec3f *)&(hullbox_set[id]->pos);
}
const vec3f * hullbox_get_ang( int id )
{
	return (const vec3f *)&(hullbox_set[id]->ang);
}
const vec3f * hullbox_get_scale( int id  )
{
	return (const vec3f *)&(hullbox_set[id]->scale);
}
const vec4f * hullbox_get_color( int id )
{
	return (const vec4f *)&(hullbox_set[id]->color);
}
