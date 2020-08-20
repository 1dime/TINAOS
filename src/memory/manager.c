#include<memory/manager.h>
#include <stdio.h>
#ifndef MEMORY_MANAGER_C
#define MEMORY_MANAGER_C

//Create the bitmap
uint8_t* bmp = (uint8_t*)(&kernel_end);
uint8_t* memory_start;
uint32_t total_blocks;
uint32_t memory_bmp_size;

void build_memory_manager(uint32_t memory_size)
{
    //Set the total block count
    total_blocks = memory_size / block_size;
     //We set the memory bitmap size so we know how many bytes are needed
     memory_bmp_size = total_blocks / blocks_per_belt;
     //Check if the bitmap size is too small
     if(memory_bmp_size * blocks_per_belt < total_blocks)
        //Increase the bitmap size by 1
        memory_bmp_size++;
    //Clear the given bitmap
    memset(bmp, 0, memory_bmp_size);
    //Set the memory start
    memory_start = (uint8_t*)align_block(((uint32_t) (bmp + memory_bmp_size)));
}

//Allocate a block for memory
uint32_t allocate_memory_block()
{
    //Get the first free block
    uint32_t free_block = get_first_free_block();
    //Use set_bit on my free block
    set_bit(free_block);
    //Now I must return the free block
    return free_block;
}

//Free the block
uint32_t free_memory_block(uint32_t memory_block_number)
{
    //Just free the memory block
    clear_bit(memory_block_number);
}

//Get first free block
uint32_t get_first_free_block()
{
    for(uint32_t i = 0; i < total_blocks; i++)
    {
        if(!is_block_set(i))
            return i;
    }
    write_warning_string("TINAOS is running out of free blocks");
    return (uint32_t) -1;
}
#endif