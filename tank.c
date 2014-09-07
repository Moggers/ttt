#include "tank.h"
#include "hullbox.h"
#include <stdlib.h>
#include <string.h>

tank_t * tank_create( void )
{
	tank_t * t = calloc( 1, sizeof( tank_t ) );
	t->hull[0] = hullbox_create();
	t->hull[1] = hullbox_create();
	t->hull[2] = hullbox_create();

	hullbox_set_scale( t->hull[0], &(vec3f){5, 1, 9 } );
	hullbox_set_scale( t->hull[1], &(vec3f){4, 1, 4 } );
	hullbox_set_scale( t->hull[2], &(vec3f){1, 1, 8 } );

	return t;
}

void tank_set_pos( tank_t * src, vec3f * arg0 )
{
	memcpy( &(src->pos), arg0, sizeof( vec3f ) );
	vec3f temp;
	memcpy( &temp, arg0, sizeof( vec3f ) );
	hullbox_set_pos( src->hull[0], &temp );
	temp[1] += 1;
	temp[2] += 2;
	hullbox_set_pos( src->hull[1], &temp );
	temp[2] -= 3;
	hullbox_set_pos( src->hull[2], &temp );
}
