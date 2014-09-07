#ifndef TANK_IG
#define TANK_IG
#include "vec3f.h"

struct tank_s
{
	int hull[3];
	vec3f pos;
	vec3f ang;
	float turret_yaw;
};
typedef struct tank_s tank_t;

tank_t * tank_create( void );
void tank_set_pos( tank_t * src, vec3f * arg0 );
void tank_set_ang( tank_t * src, vec3f * arg0 );

#endif // TANK_IG
