#ifndef SBRK_C
#define SBRK_C
#include <sys/sbrk.h>

extern void stack_top;
extern void stack_bottom;
void *g_heap = (void *)0;

void *sbrk(int nbytes)
{
    void *alloc;
    if(!g_heap) g_heap = &stack_top;
    alloc   = g_heap;
    g_heap += nbytes;
    if(g_heap >= &stack_bottom)
        return (void *)-1; // No more heap
    return alloc;
}

#endif