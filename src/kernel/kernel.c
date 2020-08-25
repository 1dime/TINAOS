#include "../display/vga.c"
#include "../drivers/ata.c"
#include "../drivers/keyboard.c"
#include <stdbool.h>
#include <stdarg.h>
extern _interupt();
int initialize_drivers()
{
    //Use this function to initialize all drivers
    write_pass_string("initialize_drivers: Initializing all drivers");
    //Initialize the ATA driver
    initiliaze_ata_driver();
    //Return nothing
    return 0;
}

int kernel_start()
{
    //Setup the terminal goods
    make_vga_terminal();
    //Write a hello
    write_string_to_terminal("TINAOS Kernel v1.0");
    //For ending kernel loop
    bool exit_kernel_loop = false;
    initialize_drivers();
    //Enter kernel loop, don't stop until commanded to do so
    while(!exit_kernel_loop)
    {
    }
}
