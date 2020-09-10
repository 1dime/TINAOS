#include <drivers/keyboard.h>


//For keycode scanning
int scan_code()
{
    //Verify that we have data passed
    if((inb(0x64) & 1) != 0)
    {
        //character =  the passed code
        return inb(0x60);
    }

    return 0; //?
}

bool key_pressed()
{
    //Return the verification of the data passed
    return (inb(0x64) & 1) != 0;
}