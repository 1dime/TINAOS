#include "../display/vga.c"
#include "../drivers/ata.c"
#include "../drivers/pci.c"
#include "../drivers/keyboard.c"
#include <stdbool.h>
#include <stdarg.h>
extern _interupt();
int initialize_drivers()
{
    //Print that we are initializing drivers
    write_current_operation("initiliaze_drivers", "Initializing all drivers");
    //Initialize all pci devices
    initialize_pci_devices();
    //Initialize the ATA driver
    initiliaze_ata_driver();
    //Return nothing
    return 0;
}

int kernel_start()
{
    //Setup the terminal goods
    make_vga_terminal();
    //For ending kernel loop
    bool exit_kernel_loop = false;
    //Write that this is the kernel
    write_pass_string("TINAOS Kernel");
    //Initialize all drivers
    initialize_drivers();
    //Enter kernel loop, don't stop until commanded to do so
    while(!exit_kernel_loop)
    {
        
    }
}
