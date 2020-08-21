#ifndef MEMORY
#define MEMORY
#include <sys/malloc.h>
#include "../sys/string.c"
#include <stdint.h>
void* calloc(uint32_t number, uint32_t size);

#endif