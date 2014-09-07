#include "render.h"
#include "debug.h"
#include "vec4f.h"
#include <stdlib.h>
#include <string.h>
#include "hullbox.h"
#include "vec3f.h"
#include <unistd.h>
#include "math.h"
#include "tank.h"

int main( int argc, char ** argv )
{
	int * render_status = render_start( 1920, 1080 );
	tank_t * t = tank_create();
	tank_set_pos( t, &(vec3f){0, 0, 0 } );
	
	int world = hullbox_create();
	render_set_box_scale( world, &(vec3f){ 300, 1, 300 } );
	render_set_box_pos( world, &(vec3f){ 0.f, -1.f, 0.f } );
	render_set_box_color( world, &(vec4f){ 0.3, 0.4, 0.3, 1.f} );

	while( *render_status < 2 )
	{
		usleep( 10000 );
	}

	pthread_exit( 0 );
}
