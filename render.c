#include <SDL2/SDL.h>
#include <stdlib.h>
#include "debug.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include "vec4f.h"
#include "math.h"
#include "vec3f.h"
#include "matrix4f.h"
#include <pthread.h>
#include "ts_queue.h"
#include "camera.h"

// Datatypes
SDL_Window * render_window = NULL;
SDL_GLContext render_gl_context = NULL;
GLuint render_vertex_shader;
GLuint render_fragment_shader;
GLuint render_program;
pthread_t * render_thread;
ts_queue * render_queue; 
int render_box_count;
float ptime, dtime, ttime;
float zoom;
vec3f cam_pos;

int render_mem_map[1024][4];

typedef struct render_com
{
	int id;
	int type; // 1 pos; 2 rot; 3 scale; 4 color; 5 kill
	void * param;
} render_com;

// Compile vertex shader
GLuint render_build_vertex_shader( void )
{
	static char * vertex_shader_source[] = {
		"#version 350 core\n"
		"struct Box\n"
		"{\n"
		"	vec4 off;\n"
		"	mat4 rot;\n"
		"	vec4 sca;\n"
		"	vec4 col;\n"
		"};\n"
		"uniform Box box_set[1024];\n"
		"uniform vec3 light_dir;\n"
		"uniform mat4 cam_rot;\n"
		"uniform vec4 cam_pos;\n"
		"void main(void)\n"
		"{\n"
		"	int cube_id = int(gl_VertexID / 36);\n"
		"	int true_vid = gl_VertexID % 36;\n"
		"	int round_vid = int(true_vid / 3 ) * 3;\n"
		"	int poly_vid = true_vid % 3;\n"
		"	const vec4 vertices[36] = vec4[36](\n"
		"		vec4( 0.5f, -0.5f, -0.5f, 1.), vec4( -0.5f, -0.5f, -0.5f, 1.), vec4( 0.5f, 0.5f, -0.5f, 1.),\n"
		"		vec4( -0.5f, 0.5f, -0.5f, 1.), vec4( 0.5f, 0.5f, -0.5f, 1.), vec4( -0.5f, -0.5f, -0.5f, 1.),\n"
		"		vec4( -0.5f, -0.5f, 0.5f, 1.), vec4( 0.5f, -0.5f, 0.5f, 1.), vec4( 0.5f, 0.5f, 0.5f, 1.),\n"
		"		vec4( 0.5f, 0.5f, 0.5f, 1.), vec4( -0.5f, 0.5f, 0.5f, 1.), vec4( -0.5f, -0.5f, 0.5f, 1.),\n"

		"		vec4( -0.5f, -0.5f, -0.5f, 1.), vec4( -0.5f, -0.5f, 0.5f, 1.), vec4( -0.5f, 0.5f, 0.5f, 1.),\n"
		"		vec4( -0.5f, 0.5f, 0.5f, 1.), vec4( -0.5f, 0.5f, -0.5f, 1.), vec4( -0.5f, -0.5f, -0.5f, 1.),\n"
		"		vec4( 0.5f, -0.5f, 0.5f, 1.), vec4( 0.5f, -0.5f, -0.5f, 1.), vec4( 0.5f, 0.5f, 0.5f, 1.),\n"
		"		vec4( 0.5f, 0.5f, -0.5f, 1.), vec4( 0.5f, 0.5f, 0.5f, 1.), vec4( 0.5f, -0.5f, -0.5f, 1.),\n"

		"		vec4( -0.5f, 0.5f, -0.5f, 1.), vec4( -0.5f, 0.5f, 0.5f, 1.), vec4( 0.5f, 0.5f, 0.5f, 1.),\n"
		"		vec4( 0.5f, 0.5f, -0.5f, 1.), vec4( -0.5f, 0.5f, -0.5f, 1.), vec4( 0.5f, 0.5f, 0.5f, 1.),\n"
		"		vec4( -0.5f, -0.5f, 0.5f, 1.), vec4( -0.5f, -0.5f, -0.5f, 1.), vec4( 0.5f, -0.5f, 0.5f, 1.),\n"
		"		vec4( -0.5f, -0.5f, -0.5f, 1.), vec4( 0.5f, -0.5f, -0.5f, 1.), vec4( 0.5f, -0.5f, 0.5f, 1.)\n"
	   	"		);\n"
		"	gl_Position = ( ( vertices[true_vid] * box_set[cube_id].sca * box_set[cube_id].rot + box_set[cube_id].off) - cam_pos) * cam_rot * gl_ProjectionMatrix;\n"
		"	if( poly_vid == 0 )\n"
		"	{\n"
		"		vec3 t0 = cross( vec3((vertices[true_vid+1] - vertices[true_vid]) * box_set[cube_id].rot), vec3((vertices[true_vid+2] - vertices[true_vid]) * box_set[cube_id].rot ) );\n"
		"		float dot = dot( normalize(t0), light_dir );\n"
		"		gl_FrontColor = box_set[cube_id].col * (( dot + 1 ) / 2);\n"
		"	}\n"
		"}\n"
	};
	GLuint vertex_shader;
	vertex_shader = glCreateShader( GL_VERTEX_SHADER );
	glShaderSource( vertex_shader, 1, vertex_shader_source, NULL );
	glCompileShader( vertex_shader );
	render_check_glsl_error( "vertex shader compilation", vertex_shader );

	return vertex_shader;
}

// Compile fragment shader
GLuint render_build_fragment_shader( void )
{
	static char * fragment_shader_source[] = {
		"#version 350 core\n"
		"out vec4 color;\n"
		"void main(void)\n"
		"{\n"
		"	color = gl_Color;\n"
		"}\n"
	};
	GLuint fragment_shader;
	fragment_shader = glCreateShader( GL_FRAGMENT_SHADER );
	glShaderSource( fragment_shader, 1, fragment_shader_source, NULL );
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
	render_check_gl_error( "creating matrices" );
}

// Increase number of rendered boxes
void render_inc_box_count()
{
	ts_queue_push( render_queue, &((render_com){ 1, 0, NULL }), sizeof( render_com ) );
}

// Decrease number of rendered boxes
void render_dec_box_count()
{
	ts_queue_push( render_queue, &((render_com){ 5, 0, NULL }), sizeof( render_com ) );
}


// Set cam pos
void render_set_cam_pos( vec3f * pos )
{
	vec3f * np = malloc( sizeof( vec3f ) );
	memcpy( np, pos, sizeof( vec3f ) );
	ts_queue_push( render_queue, &((render_com){ 0, 7, (void*)np }), sizeof( render_com ) );
}

// Set cam ang
void render_set_cam_ang( vec3f * ang )
{
	vec3f * np = malloc( sizeof( vec3f ) );
	memcpy( np, ang, sizeof( vec3f ) );
	ts_queue_push( render_queue, &((render_com){ 0, 8, (void*)np }), sizeof( render_com ) );
}

// Set box pos
void render_set_box_pos( int id, vec3f * pos )
{
	vec4f * np = malloc( sizeof( vec3f ) );
	memcpy( np, pos, sizeof( vec3f ) );
	ts_queue_push( render_queue, &((render_com){ id, 1, (void*)np }), sizeof( render_com ) );
}

// Set box angle
void render_set_box_ang( int id, vec3f * ang )
{
	vec4f * np = malloc( sizeof( vec3f ) );
	memcpy( np, ang, sizeof( vec3f ) );
	ts_queue_push( render_queue, &((render_com){ id, 2, (void*)np }), sizeof( render_com ) );
}

// Set box scale
void render_set_box_scale( int id, vec3f * scale )
{
	vec4f * np = malloc( sizeof( vec3f ) );
	memcpy( np, scale, sizeof( vec3f ) );
	ts_queue_push( render_queue, &((render_com){ id, 3, (void*)np }), sizeof( render_com ) );
}

// Set box color
void render_set_box_color( int id, vec4f * color )
{
	vec4f * np = malloc( sizeof( vec3f ) );
	memcpy( np, color, sizeof( vec3f ) );
	ts_queue_push( render_queue, &((render_com){ id, 4, (void*)np }), sizeof( render_com ) );
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

// Draw display
void render_loop( SDL_Window *win, GLuint prog )
{
	SDL_Event e;
	int state_quit = 0;
	render_com * queue_r;
	while( !state_quit )
	{
		while( SDL_PollEvent( &e ) )
		{
			switch( e.type )
			{
				case SDL_QUIT:
				{
					state_quit = 1;
					break;
				}
				case SDL_MOUSEWHEEL:
				{
					zoom *= (((-(float)e.wheel.y + 2) / 2) + 1) / 2;
					zoom = ( zoom > 1 ) ? 1 : ( zoom < 0.05 ) ? 0.05 : zoom;
					debug_printf( "zoom: %f\n", zoom );
					float z_calc = (zoom * 2 + 1) / 3 * 90;
					render_set_cam_pos( &(vec3f){ 
							cam_pos[0],
							cam_pos[1] + sinf( z_calc * ( M_PI / 180 ) ) * zoom * 200, 
							cam_pos[2] + cosf( z_calc * ( M_PI / 180 ) ) * zoom * 200});
					render_set_cam_ang( &(vec3f){ z_calc, 0.f, 0.f } );
				}
			}
		}
		while( ( queue_r = ts_queue_pop( render_queue ) ) != NULL )
		{
			vec4f np;
			switch( queue_r->type )
			{
				case 0:
					render_box_count++;
					break;
				case 1:
					if( render_mem_map[queue_r->id][0] == -1 )
					{
						char * str = calloc( 18, sizeof( char ) );
						sprintf( str, "box_set[%d].off", queue_r->id );
						render_mem_map[queue_r->id][0] = glGetUniformLocation( render_program, str );
						free( str );
					}
					memcpy( &np, queue_r->param, sizeof( vec3f ) );
					np[3] = 0;
					glUniform4fv( render_mem_map[queue_r->id][0], 1, &np );
					break;
				case 2:
					if( render_mem_map[queue_r->id][1] == -1 )
					{
						char * str = calloc( 18, sizeof( char ) );
						sprintf( str, "box_set[%d].rot", queue_r->id );
						render_mem_map[queue_r->id][1] = glGetUniformLocation( render_program, str );
						free( str );
					}
					mat4f rotation;
					mat4f_build_rotation( &rotation, (vec3f*)queue_r->param );
					glUniformMatrix4fv( render_mem_map[queue_r->id][1], 1, GL_FALSE, &rotation );
					break;
				case 3:
					if( render_mem_map[queue_r->id][2] == -1 )
					{
						char * str = calloc( 18, sizeof( char ) );
						sprintf( str, "box_set[%d].sca", queue_r->id );
						render_mem_map[queue_r->id][2] = glGetUniformLocation( render_program, str );
						free( str );
					}
					memcpy( &np, queue_r->param, sizeof( vec3f ) );
					np[3] = 1;
					glUniform4fv( render_mem_map[queue_r->id][2], 1, np );
					break;
				case 4:
					if( render_mem_map[queue_r->id][3] == -1 )
					{
						char * str = calloc( 18, sizeof( char ) );
						sprintf( str, "box_set[%d].col", queue_r->id );
						render_mem_map[queue_r->id][3] = glGetUniformLocation( render_program, str );
						free( str );
					} 
					glUniform4fv( render_mem_map[queue_r->id][3], 1, (GLuint*)queue_r->param );
					break;
				case 5:
					render_box_count--;
					break;
				case 6:
					state_quit = 1;
				case 7:
					camera_move( render_program, (vec3f*)queue_r->param );
				case 8:
					camera_rotate( render_program, (vec3f*)queue_r->param );
				default:
					break;

			}
			free( queue_r->param );
			free( queue_r );
		}
		ptime = ttime;
		ttime = clock();
		dtime = (ttime - ptime ) / 400;

		glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT );
		glDrawArrays( GL_TRIANGLES, 0, 36 * render_box_count );
		SDL_GL_SwapWindow( win );
	}
}

// Initialize all opengl facets
void * render_init( void * args )
{
	*((int*)args) = 1;
	// Window & Context
	SDL_Init( SDL_INIT_EVERYTHING );
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
	render_window = SDL_CreateWindow( "Tic Tac Toe!", 0, 0, 1920, 1080, SDL_WINDOW_OPENGL | SDL_WINDOW_MAXIMIZED | SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN );
	render_check_sdl_error( "SDL window creation" );

	render_gl_context = SDL_GL_CreateContext( render_window );
	glEnable( GL_DEPTH_TEST );
	glEnable( GL_CULL_FACE );
	glDepthFunc( GL_LEQUAL );
	glShadeModel( GL_FLAT );
	glProvokingVertex( GL_FIRST_VERTEX_CONVENTION );
	render_check_gl_error( "initiating GL properties" );

	// Shaders
	render_vertex_shader = render_build_vertex_shader();
	render_fragment_shader = render_build_fragment_shader();

	// Program
	render_program = render_build_program( render_vertex_shader, render_fragment_shader );
	glUseProgram( render_program );

	// Camera
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	gluPerspective( 40, 16.f/9.f, 1, 512 );
	render_set_cam_pos( &(vec3f){ 0.f, 0.f, 0.f } );
	render_set_cam_ang( &(vec3f){ 0.f, 0.f, 0.f } );
	zoom = 1;

	// Load light source
	glUniform3fv( glGetUniformLocation( render_program, "light_dir" ), 1, &((vec3f){ 0.f, 1.f, 0.f } ) );

	// Locked and loaded, right?
	render_check_gl_error( "video init" );

	// Test
	render_loop( render_window, render_program );

	render_destroy();
	*((int*)args) = 2;
	return NULL;
}

void * render_start( int customwidth, int customheight )
{
	memset( &(render_mem_map)[0][0], -1, sizeof( int ) * 1024 * 4 );
	render_queue = calloc( 1, sizeof( ts_queue ) );
	ts_queue_init( render_queue );

	int * status = malloc( sizeof( int ) );
	*status = 1;
	render_thread = malloc( sizeof( pthread_t ) );
	pthread_create( render_thread, NULL, render_init, status );
	return status;
}

