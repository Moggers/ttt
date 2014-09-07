#ifndef MAT4F_IG
#define MAT4F_IG
typedef float mat4f[4][4];
static const mat4f mat4f_identity;
void mat4f_product( mat4f * src0, mat4f * src1 );
void mat4f_rotate( mat4f * a, float pitch, float yaw, float roll );
char * mat4f_string( mat4f * a );
#endif
