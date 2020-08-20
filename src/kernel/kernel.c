
#include <display/vga.h>
#include <memory.h>
#include <drivers/keyboard.h>
#include "../drivers/pci.c"
#include "../memory/manager.c"
#include <stdio.h>
#include "../drivers/ata.c"

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
