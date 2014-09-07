#ifndef CAMERA_IG
#define CAMERA_IG
#include "vec3f.h"
#include <GL/gl.h>
void camera_move( GLuint program, vec3f * pos );
void camera_rotate( GLuint program, vec3f * ang );
#endif
