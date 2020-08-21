#include "../display/vga.c"
#include "../drivers/ata.c"
#include <stdbool.h>

void initialize_drivers()
{
    //Use this function to initialize all drivers

    //Initialize the ATA driver
    initiliaze_ata_driver();
}

int kernel_start()
{
    //Setup the terminal goods
    make_vga_terminal();
    //Write a hello
    write_string_to_terminal("TINAOS Kernel v1.0\n");
    //For ending kernel loop
    bool exit_kernel_loop = false;
    //Initialize all drivers
    initialize_drivers();
    //Enter kernel loop, don't stop until commanded to do so
    while(!exit_kernel_loop)
    {
        
    }
}
