#ifndef INTERRUPTS
#define INTERRUPTS
int inb(unsigned int port)
{
    int val;
    asm volatile ("inb %%dx,%%al":"=a" (val): "d" (port));
    return val;
}

void outb(unsigned int port, unsigned char value)
{
    asm volatile("outb %%al,%%dx": : "d" (port), "a" (value));
}
#endif