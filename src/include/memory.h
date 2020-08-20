#ifndef MEMORY
#define MEMORY
#include <sys/malloc.h>
#include <memory/pager.h>
extern uint32_t kernel_end;
extern pager_directory_t *pager_dir;
uint32_t placement_address = (uint32_t)&kernel_end;

void* calloc(uint32_t number, uint32_t size)
{
    void *custom_ptr = malloc(number * size);
    memset(custom_ptr, 0, number * size);
    return custom_ptr;
}


void * heap_start;
void * heap_end;
void * heap_top;
void * heap_max;

extern void * heap_start, * heap_end, *heap_top;

uint32_t malloc_a(uint32_t size, int align, uint32_t *phys)
{
    if(heap_start != NULL)
    {
        if(align) size = size + 4096;
        void *address = malloc(size);
        uint32_t align_address = ((uint32_t)address & 0xFFFFF000) + 0x1000;
        if(phys != 0)
        {
            uint32_t addr = (uint32_t) address;
            if(align)
                addr = align_address;
            *phys = (uint32_t) virt_2_phys(pager_dir, (void*) addr);
        }
        if(align)
            return align_address;
        return (uint32_t) address;
    }
    else
    {
        if(align == 1 && (placement_address & 0xFFFFF000))
        {
            placement_address &= 0xFFFFF000;
            placement_address += 0x1000;
        }
        if (phys)
        {
            *phys = placement_address;
        }
        uint32_t temp = placement_address;
        placement_address += size;
        return temp;
    }
}

void *malloc_4k(unsigned int size)
{
    return malloc_a(size, 1, 0);
}
#endif