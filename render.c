#include <SDL2/SDL.h>
#include <string.h>
#include <stdlib.h>
#include "debug.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include "matrix4f.h"

// Datatypes
SDL_Window * render_window = NULL;
SDL_GLContext render_gl_context = NULL;
GLuint render_vertex_shader;
GLuint render_fragment_shader;
GLuint render_program;


// Compile vertex shader
GLuint render_build_vertex_shader( void )
{
	static char * vertex_shader_source[] = {
		"#version 430 core\n"
		"layout ( location = 0 ) uniform mat4 gWorld;\n"
		"void main(void)\n"
		"{\n"
		"	const vec4 vertices[6] = vec4[6](\n"
		"		vec4( -0.1, -0.1, 0, 1.0 ), vec4( 0.1, -0.1, 0, 1.0 ), vec4( 0.1, 0.1, 0, 1.0 ),\n"
		"		vec4( -0.1, 0.1, 0, 1.0 ), vec4( 0.1, 0.1, 0, 1.0 ), vec4( -0.1, -0.1, 0, 1.0 )\n"
	   	"		);\n"
		"	vec4 offset = vec4( 0.0, 0.0, -1, 0 );\n"
		"	gl_Position = (gWorld * vertices[gl_VertexID] + offset) * gl_ModelViewProjectionMatrix;\n"
		//"	gl_Position = vertices[gl_VertexID];\n"
		"}\n"
	};
	GLuint vertex_shader;
	vertex_shader = glCreateShader( GL_VERTEX_SHADER );
	render_check_gl_error( "vertex shader creation" );

	glShaderSource( vertex_shader, 1, vertex_shader_source, NULL );
	render_check_gl_error( "vertex shader source insertion" );

	glCompileShader( vertex_shader );
	render_check_glsl_error( "vertex shader compilation", vertex_shader );

	return vertex_shader;
}

// Compile fragment shader
GLuint render_build_fragment_shader( void )
{
	static char * fragment_shader_source[] = {
		"#version 430 core\n"
		"out vec4 color;\n"
		"void main(void)\n"
		"{\n"
		"	color = vec4( 0.0, 1.0, 0.0, 1.0 );\n"
		"}\n"
	};
	GLuint fragment_shader;
	fragment_shader = glCreateShader( GL_FRAGMENT_SHADER );
	render_check_gl_error( "fragment shader creation" );

	glShaderSource( fragment_shader, 1, fragment_shader_source, NULL );
	render_check_gl_error( "fragment shader source insertion" );

	glCompileShader( fragment_shader );
	render_check_glsl_error( "fragment shader compilation", fragment_shader );

	return fragment_shader;
}

// Link program
GLuint render_build_program( GLuint vert_sh, GLuint frag_sh )
{
	GLuint prog = glCreateProgram();
	render_check_gl_error( "GLSL program creation" );

	glAttachShader( prog, vert_sh );
	render_check_gl_error( "attaching vertex shader to program" );

	glAttachShader( prog, frag_sh );
	render_check_gl_error( "attaching fragment shader to program" );

	glLinkProgram( prog );
	render_check_gl_error( "linking GLSL program" );

	glDeleteShader( vert_sh );
	glDeleteShader( frag_sh );

	return prog;
}

// Build projection matrices, viewmodel, etc
void render_build_matrices( void )
{
	gluPerspective( 90, (GLfloat)16/(GLfloat)10, 1, 100 );
	render_check_gl_error( "creating matrices" );
}

// Draw display
void render_display( SDL_Window *win, GLuint prog )
{
	glClearBufferfv( GL_COLOR, 0, (GLfloat[]){ 0.0f, 0.0f, .1f } );
	glUseProgram( prog );
	render_check_gl_error( "using GLSL program in draw test" );
	mat4f * translation = malloc( sizeof( mat4f ) );
	mat4f_identity( translation );
	GLint g_world = glGetUniformLocation( prog, (GLchar*)"gWorld" );
	glUniformMatrix4fv( g_world, 1, GL_TRUE, translation );
	render_check_gl_error( "loading translation matrix" );
	glDrawArrays( GL_TRIANGLES, 0, 6 );
	SDL_GL_SwapWindow( win );
	render_check_gl_error( "draw test" );
	getchar();
}

// Initialize all opengl facets
void render_init( int customwidth, int customheight )
{

	// Window & Context
	SDL_Init( SDL_INIT_EVERYTHING );
	render_check_sdl_error( "SDL init" );
	SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );

	render_window = SDL_CreateWindow( "Tic Tac Toe!", 0, 0, 1920, 1080, SDL_WINDOW_FULLSCREEN_DESKTOP | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN );
	render_check_sdl_error( "window creation" );

	render_gl_context = SDL_GL_CreateContext( render_window );
	render_check_sdl_error( "opengl context" );

	// Shaders
	render_vertex_shader = render_build_vertex_shader();
	render_fragment_shader = render_build_fragment_shader();

	// Program
	render_program = render_build_program( render_vertex_shader, render_fragment_shader );

	// Camera
	render_build_matrices();

	// Test
	render_display( render_window, render_program );
}

// SHUT IT DOWN
void render_destroy( void )
{
	//Destruction
	SDL_DestroyWindow( render_window );
	SDL_GL_DeleteContext( render_gl_context );
	SDL_Quit();
	render_check_sdl_error( "shutdown" );
}
