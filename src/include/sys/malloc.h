
#ifndef MALLOC
#define MALLOC
#define BLOCK 1024

#include <sys/sbrk.h>
static union header
{
  struct
  {
    union header *link;
    unsigned size;
  } s;
  union align
  {
    double d;
    unsigned u;
    void (*f) (void);
  } x;
} freelist ={&freelist, 0}, *freep = &freelist;
 
void *malloc(unsigned nbytes);
void free(void *ptr);
#endif