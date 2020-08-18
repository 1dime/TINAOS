#include <interrupts/interrupts.h>
int scan_code()
{
    //Verify that we have data passed
    if((inb(0x64) & 1) != 0)
    {
        //Return the passed code
        return inb(0x60);
    }
}

char translate_code(int code)
{
    //This function is to convert the given character code to a char
}