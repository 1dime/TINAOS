
#include <display/vga.h>
#include <drivers/keyboard.h>
void kernel_start()
{
    //Setup the terminal goods
    make_vga_terminal();
    //Write a hello
    write_string_to_terminal("TINAOS Kernel v1.0\n");
    //Loop, and don't let go, jack
    while(1)
    {
        //Check if user has pressed something
        if((inb(0x64) & 1) != 0)
        {
            //Excuse me, if I may interrupt, what did you type?
            int code = scan_code();
            //Oh okay lol
            write_string_to_terminal("User has made keyboard input!\n");
        }

    }
}
