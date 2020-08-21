#ifndef MEMORY_C
#define MEMORY_C
#include <sys/memory.h>

void* calloc(uint32_t number, uint32_t size)
{
    void *_ptr = malloc(number * size);
    memset(_ptr, 0, number * size);
    return _ptr;
}

#endif