#include "camera.h"
#include "matrix4f.h"
#include "vec4f.h"
#include <string.h>
#include <stdlib.h>

GLuint cam_mem_pos;
GLuint cam_mem_ang;
vec4f camera_cam_pos;
mat4f camera_cam_rot;

void camera_move( GLuint program, vec3f * pos )
{
	if( cam_mem_pos == 0 )
		cam_mem_pos = glGetUniformLocation( program, "cam_pos" );
	memcpy( &camera_cam_pos, pos, sizeof( vec3f ) );
	glUniform4fv( cam_mem_pos, 1, &camera_cam_pos );
		
}

void camera_rotate( GLuint program, vec3f * ang )
{
	if( cam_mem_ang == 0 )
		cam_mem_ang = glGetUniformLocation( program, "cam_rot" );
	mat4f_build_rotation( &camera_cam_rot, ang );
	glUniformMatrix4fv( cam_mem_ang, 1, GL_FALSE, &camera_cam_rot );
}
