#ifndef MAT4F_IG
#define MAT4F_IG
typedef float mat4f[4][4];
void mat4f_product( mat4f * dest, mat4f * src0, mat4f * src1 );
void mat4f_identity( mat4f * dest );
void mat4f_rotate( mat4f * a, float pitch, float yaw, float roll );
#endif
