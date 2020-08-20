#ifndef BIOS
#define BIOS
//I need these structs to get info from the bios
typedef struct bios_register
{
    uint16_t di;
    uint16_t si;
    uint16_t bp;
    uint16_t sp;
    uint16_t bx;
    uint16_t dx;
    uint16_t cx;
    uint16_t ax;

    uint16_t ds;
    uint16_t es;
    uint16_t fs;
    uint16_t gs;
    uint16_t ss;
    uint16_t eflags;
} bios_register_t;


uint8_t bios_in_mem_b(uint32_t address)
{
    return *((uint8_t*)(address));
}

uint16_t bios_in_mem_s(uint32_t address)
{
    return *((uint16_t*)(address));
}

uint32_t bios_in_mem_l(uint32_t address)
{
    return *((uint32_t*)(address));
}
void out_mem_b(uint32_t address, uint8_t value)
{
    (*((uint8_t*)(address))) = value;
}

void out_mem_s(uint32_t address, uint16_t value)
{
    (*((uint16_t*)(address))) = value;
}


void out_mem_l(uint32_t address, uint32_t value)
{
    (*((uint32_t*)(address))) = value;
}
#endif