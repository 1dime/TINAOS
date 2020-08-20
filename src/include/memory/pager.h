#ifndef PAGER
#define PAGER

#include <stdint.h>
#include <interrupts/interrupts.h>
#include <memory.h>
#include <bios/bios.h>

//arbitrary page size
#define PAGER_PAGE_SIZE 4096

//Macros related to page alignment
#define is_page_aligned(addr)((((uint32_t) (addr)) | 0xFFFFF000) == 0)
#define align_page(addr)((((uint32_t) (addr)) & 0xFFFFF000) + 0x1000)

//Address-defining macros
#define pager_pagedir_index(vaddr) (((uint32_t)vaddr) >> 22)
#define pager_pagetable_index(vaddr)((((uint32_t)vaddr) >> 12) & 0x3ff)
#define pager_pageframe_index(vaddr)(((uint32_t)vaddr) & 0xfff)

//This is useful for register manipulation
#define pager_set_pagebit(page) (page = page | 0x80000000)
#define pager_clear_page_size_extension_bit(page_bit) ( page_bit = page_bit & 0xffffffef)

//In the case we have an error
#define PAGER_PRESENT_ERROR 0x1
#define PAGER_READ_WRITE_ERROR 0x2
#define PAGER_USER_ERROR 0x4
#define PAGER_RESERVED_ERROR 0x8
#define PAGER_INSTANCE_ERROR 0x10

#define LOAD_MEMORY_ADDRESS 0xC0000000
typedef struct pager_dir_entry
{
    unsigned int present : 1;
    unsigned int read_write : 1;
    unsigned int user : 1;
    unsigned int write_through : 1;
    unsigned int cache : 1;
    unsigned int access : 1;
    unsigned int reserved : 1;
    unsigned int page_size : 1;
    unsigned int global : 1;
    unsigned int available : 3;
    unsigned int frame : 20;
} pager_dir_entry_t;

typedef struct pager_table_entry
{
    unsigned int present : 1;
    unsigned int read_write : 1;
    unsigned int user : 1;
    unsigned int reserved : 1;
    unsigned int accessed : 1;
    unsigned int dirty : 1;
    unsigned int secondary_reserved : 2;
    unsigned int available: 3;
    unsigned int frame: 20;
} pager_table_entry_t;

typedef struct pager_table
{
    pager_table_entry_t pages[1024];
} page_table_t;

typedef struct pager_directory
{
    pager_dir_entry_t tables[1024];
    page_table_t* address_tables[1024];
} pager_directory_t;

//Get the temporary page directory
extern pager_directory_t tmp_page_dir;

//Set the bitmap
extern uint8_t* bmp;
//Get the bitmap size
extern uint32_t memory_bmp_size;

extern pager_directory_t *pager_dir;
pager_directory_t* pager_dir;
int paging_enabled = 0;
void * virt_2_phys(pager_directory_t * dir, void * virtual_addr)
{
    if(!paging_enabled)
    {
        return (void*)(virtual_addr - LOAD_MEMORY_ADDRESS);
    }
    uint32_t page_dir_idx = pager_pagedir_index(virtual_addr), page_table_idx = pager_pagetable_index(virtual_addr), page_frame_off = pager_pageframe_index(virtual_addr);
    if(!dir->address_tables[page_dir_idx])
    {
        write_error_string("virt_2_phys: No page directory entry exists\n");
        return NULL;
    }

    page_table_t* table = dir->address_tables[page_dir_idx];
    if(!table->pages[page_table_idx].present)
    {
        write_error_string("virt_2_phys: Memory page table does not exist\n");
        return NULL;
    }

    uint32_t new_table = table->pages[page_table_idx].frame;
    new_table = (new_table << 12) + page_frame_off;
    return (void*) new_table;
}
#endif