#include "../display/vga.c"
#include <stdbool.h>
int kernel_start()
{
    //Setup the terminal goods
    make_vga_terminal();
    //Write a hello
    write_string_to_terminal("TINAOS Kernel v1.0\n");
    bool exit_kernel_loop = false;
    while(!exit_kernel_loop)
    {
        
    }
}
