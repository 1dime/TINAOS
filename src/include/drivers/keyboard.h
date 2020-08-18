#ifndef KEYBOARD
#define KEYBOARD
#include <interrupts/interrupts.h>
#include <stdbool.h>

int last_key_code = 0x00;
int scan_code()
{
    //Verify that we have data passed
    if((inb(0x64) & 1) != 0)
    {
        //character =  the passed code
        return inb(0x60);
    }
}

char send_character(int code)
{
    switch(code)
    {
        case 0x46:
            write_string_to_terminal("ScrollLock");
            break;
    }
    return ' ';
}

bool key_event_occurred()
{
    if((inb(0x64) & 1) != 0)
        //A key event had occurred
        return true;
    //No key event occurred
    return false;
}

#endif