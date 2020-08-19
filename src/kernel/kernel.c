
#include <display/vga.h>
#include <drivers/keyboard.h>
#include <fcntl.h>
int kernel_start()
{
    //Setup the terminal goods
    make_vga_terminal();
    //Write a hello
    write_string_to_terminal("TINAOS Kernel v1.0\n");
    //First loop is for checking if user wants to enter debug mode
    while(true)
    {
        if(key_event_occurred())
        {
            int code = scan_code();
            send_character(code);
        }
    }
}
