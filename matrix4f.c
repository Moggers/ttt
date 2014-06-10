#include "matrix4f.h"
#include <string.h>
#include <stdlib.h>

void mat4f_product( mat4f * dest, mat4f * src0,  mat4f * src1 )
{
	int i,t,k;
	for( t = 0; t < 4; t++ )
	{
		for( k = 0; k < 4; k++ )
		{
			for( i = 0; i < 4; i++ )
			{
				*dest[t][k] += *src0[t][i] * *src1[i][k];
			}
		}
	}
}

void mat4f_identity( mat4f * dest )
{
	memset( dest, 0, sizeof( mat4f ) );
	int i;
	for( i = 0; i < 4; i++ )
		*dest[i][i] = 1;
}

void mat4f_rotate( mat4f * a, float pitch, float yaw, float roll )
{

}
