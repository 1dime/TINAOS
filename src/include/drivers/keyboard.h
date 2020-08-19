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
    char character = ' ';
    switch(code)
    {
        case 0x02:
            put_terminal_char('1');
            character = '1';
            break;
        case 0x03:
            put_terminal_char('2');
            character = '2';
            break;
    }
    return character;
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