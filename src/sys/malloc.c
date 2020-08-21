#ifndef MALLOC_C
#define MALLOC_C
#include <sys/malloc.h>
#include "sbrk.c"

void *malloc(unsigned nbytes)
{
     union header *p, *q;
     unsigned size = (nbytes + sizeof (union header) - 1) / sizeof (union header) + 1;
     q = freep;
     do
     {
         p = q->s.link;
         if (p->s.size > size)
         {
            p->s.size -= size;
            p += p->s.size;
            p->s.size = size;
            freep = q;
            return p + 1;
         }
         else if (p->s.size == size)
         {
            q->s.link = p->s.link;
            freep = q;
            return p + 1;
         }
         q = p;
     }
     while (p != freep);
   
     if (size < BLOCK) size = BLOCK;
     p = sbrk (size * sizeof (*p));
     if (p == (void *) -1) return 0;
     p->s.size = size;
 
     p->s.link = freep->s.link;
     freep->s.link = p;
 
     return malloc (nbytes);
}
 
void free(void *ptr)
{
     union header *bp = (union header *) ptr - 1, *p;
     if (ptr == 0) return;
     for (p = freep;; p = p->s.link)
         if (bp > p && bp < p->s.link || p >= p->s.link && (bp > p || bp < p->s.link)) break;
     if (bp + bp->s.size == p->s.link)
     {
        bp->s.size += p->s.link->s.size;
        bp->s.link = p->s.link->s.link;
     }
     else bp->s.link = p->s.link;
      
     if (p + p->s.size == bp)
     {
        p->s.size += bp->s.size;
        p->s.link = bp->s.link;
     }
     else p->s.link = bp;
     freep = p;
}
#endif