#ifndef DEBUG_IG
#define DEBUG_IG
#ifdef DEBUG
#include <stdio.h>
#define debug_printf( ... ) fprintf( stdout, __VA_ARGS__ )
#else
#define debug_printf( ... )
#endif
#endif
