#ifndef MEMORY_MANAGER
#define MEMORY_MANAGER
#include <interrupts/interrupts.h>
#include <stdint.h>

#define block_size 4096
#define blocks_per_belt 8
#define set_bit(i) bmp[i / blocks_per_belt] = bmp[i / blocks_per_belt] | (1 << (i % blocks_per_belt))
#define clear_bit(i) bmp[i / blocks_per_belt] = bmp[i /blocks_per_belt] & (~(1 << (i % blocks_per_belt)))
#define is_block_set(i) ((bmp[i / blocks_per_belt] >> (i % blocks_per_belt)) & 0x1)
#define get_32_bucket(i)(*((uint32_t*) &bmp[i / 32]))

//Memory block alignment
#define align_block(addr)(((addr) & 0xFFFFF000) + 0x1000)

extern uint32_t kernel_end;

void build_memory_manager(uint32_t memory_size);
//Allocate a block for memory
uint32_t allocate_memory_block();
//Free the block
uint32_t free_memory_block(uint32_t memory_block_number);
//Get first free block
uint32_t get_first_free_block();
#endif