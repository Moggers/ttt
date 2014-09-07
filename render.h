#ifndef RENDER_IG
#define RENDER_IG
#include "vec3f.h"
void * render_start( int customheight, int customwidth );
void render_set_box_pos( int id, vec3f * pos );
void render_set_box_ang( int id, vec3f * ang );
void render_set_box_scale( int id, vec3f * scale );
void render_set_cam_pos( vec3f * pos );
void render_set_cam_ang( vec3f * ang );
#endif
