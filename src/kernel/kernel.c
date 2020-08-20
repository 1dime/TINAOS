
#include <stddef.h>
#include <memory.h>
#include <string.h>
#include <display/vga.h>
#include <interrupts/interrupts.h>
#include <stdbool.h>
#include <drivers/pci.h>
#include "../drivers/pci.c"
#include <drivers/ata.h>
#include "../drivers/ata.c"
#include "../vfs/vfs.c"
#include "../drivers/ata.c"

#include <sys/fcntl.h>
int kernel_start()
{
    //Setup the terminal goods
    make_vga_terminal();
    //Write a hello
    write_string_to_terminal("TINAOS Kernel v1.0\n");
    //First loop is for checking if user wants to enter debug mode
    while(true)
    {
    }
}
