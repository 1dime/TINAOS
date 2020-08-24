#ifndef INTERRUPTS
#define INTERRUPTS
#include <stdint.h>
int inb(uint16_t port);
uint32_t inl(uint16_t _port);
uint16_t ins(uint16_t _port);

inline void outb(uint16_t port, uint8_t value);
void outs(uint16_t _port, uint16_t _data);
void outl(uint16_t _port, uint32_t _data);
#endif