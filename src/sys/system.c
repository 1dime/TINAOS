#ifndef INTERRUPTS_C
#define INTERRUPTS_C
#include <sys/system.h>
int inb(uint16_t port)
{
    uint8_t ret;
    asm volatile ( "inb %1, %0"
                   : "=a"(ret)
                   : "Nd"(port) );
    return ret;
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

inline void outb(uint16_t port, uint8_t value)
{
    asm volatile("outb %0, %1": : "a" (value), "Nd" (port));
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