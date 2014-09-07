#ifndef STACK_IG
#define STACK_IG
typedef int stack[1024];

int stack_pop( stack * src );
void stack_push( stack * dest, int src1 );
#endif
