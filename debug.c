#include "debug.h"

#include <GL/gl.h>
#include <SDL2/SDL.h>
#include <GL/glu.h>
void render_check_gl_error( char * a )
{
#ifdef DEBUG
	GLenum b = glGetError();
	if( b == GL_NO_ERROR )
		fprintf( stdout, "Finished %s\n", a );
	else
		fprintf( stderr, "Failed %s: %s\n", a, gluErrorString( b ) );
#endif
}

void render_check_glsl_error( char * a, GLuint shader )
{
#ifdef DEBUG
	GLenum b = glGetError();
	if( b == GL_NO_ERROR )
		fprintf( stdout, "Finished %s\n", a );
	else
	{
		GLsizei length;
		GLchar log[4096];
		glGetShaderInfoLog( shader, 4096, &length, log );
		debug_printf( "Fragment shader log: %s\n", log );
		fprintf( stderr, "Failed %s:\n%s\n", a, log );
	}
#endif
}

void render_check_sdl_error( char * a )
{
#ifdef DEBUG
	if( strlen( SDL_GetError() ) == 0 )
		fprintf( stdout, "Finished %s\n", a );
	else
		fprintf( stderr, "Failed %s: %s\n", a, SDL_GetError() );
#endif
}
