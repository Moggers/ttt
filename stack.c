#include "stack.h"

int stack_pop( stack * src )
{
	if( (*src)[0] == 0 )
		return 0;
	return (*src)[(*src)[0]--];
}

void stack_push( stack * dest, int src )
{
	(*dest)[++(*dest)[0]] = src;
}
