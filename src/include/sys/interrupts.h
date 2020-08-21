#ifndef INTERRUPTS
#define INTERRUPTS
#include <stdint.h>
int inb(unsigned int port);
uint32_t inl(uint16_t _port);
uint16_t ins(uint16_t _port);

void outb(unsigned int port, unsigned char value);
void outs(uint16_t _port, uint16_t _data);
void outl(uint16_t _port, uint32_t _data);
#endif