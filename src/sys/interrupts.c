#ifndef INTERRUPTS_C
#define INTERRUPTS_C
#include <sys/interrupts.h>
int inb(unsigned int port)
{
    int val;
    asm volatile ("inb %%dx,%%al":"=a" (val): "d" (port));
    return val;
}

uint32_t inl(uint16_t _port) 
{
    uint32_t rv;
    asm volatile ("inl %%dx, %%eax" : "=a" (rv) : "dN" (_port));
    return rv;
}


uint16_t ins(uint16_t _port) 
{
    uint16_t rv;
    asm volatile ("inw %1, %0" : "=a" (rv) : "dN" (_port));
    return rv;
}

void outb(unsigned int port, unsigned char value)
{
    asm volatile("outb %%al,%%dx": : "d" (port), "a" (value));
}

void outs(uint16_t _port, uint16_t _data) 
{
    asm volatile ("outw %1, %0" : : "dN" (_port), "a" (_data));
}

void outl(uint16_t _port, uint32_t _data) 
{
    asm volatile ("outl %%eax, %%dx" : : "dN" (_port), "a" (_data));
}
#endif