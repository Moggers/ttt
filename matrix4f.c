#include "matrix4f.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "debug.h"
#include <math.h>
#include "vec3f.h"

static const mat4f mat4f_identity = { 	
	1, 0, 0, 0, 
	0, 1, 0, 0, 
	0, 0, 1, 0, 
	0, 0, 0, 1 };

void mat4f_product( mat4f * src0,  mat4f * src1 )
{
	mat4f * temp = calloc( 1, sizeof( mat4f ) );
	int i,t,k;
	for( t = 0; t < 4; t++ )
	{
		for( k = 0; k < 4; k++ )
		{
			for( i = 0; i < 4; i++ )
			{
				(*temp)[t][k] += (*src0)[t][i] * (*src1)[i][k];
			}
		}
	}
	memcpy( src0, temp, sizeof( mat4f ) );
}

void mat4f_build_rotation( mat4f * dest, vec3f * ang )
{
	mat4f * t =  malloc( sizeof( mat4f ) );

	memcpy( dest, mat4f_identity, sizeof( mat4f  ) );
	(*dest)[1][1] = cosf( -(*ang)[0] * ( M_PI / 180 ) );
	(*dest)[1][2] = sinf( -(*ang)[0] * ( M_PI / 180 ) );
	(*dest)[2][1] = -sinf( -(*ang)[0] * ( M_PI / 180 ) );
	(*dest)[2][2] = cosf( -(*ang)[0] * ( M_PI / 180 ) );

	memcpy( t, mat4f_identity, sizeof( mat4f ) );
	(*t)[0][0] = cosf( -(*ang)[1] * ( M_PI / 180 ) );
	(*t)[2][0] = sinf( -(*ang)[1] * ( M_PI / 180 ) );
	(*t)[0][2] = -sinf( -(*ang)[1] * ( M_PI / 180 ) );
	(*t)[2][2] = cosf( -(*ang)[1] * ( M_PI / 180 ) );
	mat4f_product( dest, t );

	memcpy( t, mat4f_identity, sizeof( mat4f ) );
	(*t)[0][0] = cosf( -(*ang)[2] * ( M_PI / 180 ) );
	(*t)[1][0] = sinf( -(*ang)[2] * ( M_PI / 180 ) );
	(*t)[0][1] = -sinf( -(*ang)[2] * ( M_PI / 180 ) );
	(*t)[1][1] = cosf( -(*ang)[2] * ( M_PI / 180 ) );
	mat4f_product( dest, t );
}

char * mat4f_string( mat4f * a )
{
	char * s = calloc( 256, sizeof( char ) );
	sprintf( s, "%.2f, %.2f, %.2f, %.2f\n%.2f, %.2f, %.2f, %.2f\n%.2f, %.2f, %.2f, %.2f\n%.2f, %.2f, %.2f, %.2f\n", (*a)[0][0],(*a)[0][1],(*a)[0][2],(*a)[0][3],(*a)[1][0],(*a)[1][1],(*a)[1][2],(*a)[1][3],(*a)[2][0],(*a)[2][1],(*a)[2][2],(*a)[2][3],(*a)[3][0],(*a)[3][1],(*a)[3][2],(*a)[3][3] );
	return s;
}
