#ifndef HULLBOX_IG
#define HULLBOX_IG
#include "vec3f.h"
#include "vec4f.h"
typedef struct hullbox
{
	vec3f pos;
	vec3f ang;
	vec3f scale;
	vec4f color;
	int gl_stack_position;
} hullbox;

typedef hullbox *hullbox_list[1024];

int hullbox_create( void );
void hullbox_delete( int id );

int hullbox_set_pos( int id, vec3f * src );
int hullbox_set_ang( int id, vec3f * src );
int hullbox_set_scale( int id, vec3f * src );
int hullbox_set_color( int id, vec4f * src );

const vec3f * hullbox_get_pos( int id );
const vec3f * hullbox_get_ang( int id );
const vec3f * hullbox_get_scale( int id  );
const vec4f * hullbox_get_color( int id );
#endif
