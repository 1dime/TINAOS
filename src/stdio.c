#ifndef STDIO_C
#define STDIO_C
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <display/vga.h>
//Define kernel panic
#define krnl_panic(...){make_vga_terminal();cprintf(VGA_COLOR_RED, "[KERNEL_PANIC] => Kernel panic in %s at function %s", __FILE__, __LINE__, __func__);cprintf(VGA_COLOR_RED, __VA_ARGS__);for(;;);}

char tbuf[32];
char bchars[] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

void htoc(uint16_t hex)
{
    printf("0x");
    put_terminal_char(bchars[(hex & 0xF0) >> 4]);
    put_terminal_char(bchars[(hex & 0x0F) >> 0]);
}

inline int8_t isdigit(char c)
{
    return c >= '0' && c <= '9';
}

inline int8_t isalpha(char c)
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

inline int8_t isalnum(char c)
{
    return isdigit(c) || isalpha(c);
}

void itoa(unsigned i,unsigned base,char* buf) {
	int pos = 0;
	int opos = 0;
	int top = 0;

	if (i == 0 || base > 16) {
		buf[0] = '0';
		buf[1] = '\0';
		return;
	}

	while (i != 0) {
		tbuf[pos] = bchars[i % base];
		pos++;
		i /= base;
	}
	top=pos--;
	for (opos=0; opos<top; pos--,opos++)
		buf[opos] = tbuf[pos];
	buf[opos] = 0;
}

int32_t atoi(const char* s)
{
	uint8_t len = (uint8_t) strlen(s);
	int32_t result = 0;
	uint32_t mul = 1;

	while (len > 0 && isdigit(s[len - 1])) //loop from right to left
	{
		result += (s[--len] - '0') * mul;
		mul *= 10;
	}

	if (len == 1 && s[0] == '-') //negative number
	{
		result *= -1;
        len = 0; //completed
	}

    if (len == 0)
    {
        return result; //valid number
    }
	
    return -1; //invalid string
}
void cprintf(uint8_t color, char* fmt, ...)
{
    uint8_t old_color = terminal_color;
    terminal_color = vga_color_entry(color, VGA_COLOR_WHITE);
    
    va_list ap;
    char *p, *sval;
    int ival;
    double dval;

    va_start(ap, fmt);
    for(p = fmt; *p; p++) {
        if(*p != '%') {
            put_terminal_char(*p);
            continue;
        }
        switch (*++p) {
            case 'c':
                sval = (char*)(va_arg(ap, int) & ~0xFFFFFF00);
                put_terminal_char((int)sval);
                break;
            case 'd':
                ival = va_arg(ap, int);
                printf("%f", ival );
                break;
            case 'f':
                dval  = va_arg(ap, double);
                printf("%f", dval);
                break;
            case 's':
                for(sval = va_arg(ap, char *); *sval; sval++)
                    put_terminal_char(*sval);
                break;
            case 'x':
                //Get next argument
                ival = va_arg(ap, int);
                //Prepare a 32 character long string
                char str[32] = {0};
                //Convert the integer ival to a string
                itoa(ival, 16, str);
                //Write the integer to the screen
                write_to_terminal(str, strlen(str));
                break;
            default:
                put_terminal_char(*p);
                break;
        }
    }
    if(NEW_LINE_AT_END)
        //Add new line
        put_terminal_char('\n');
    va_end(ap); 
    terminal_color = old_color;
}

//Print a string with a given format
void printf(char* fmt, ...)
{
    va_list ap;
    char *p, *sval;
    int ival;
    double dval;

    va_start(ap, fmt);
    for(p = fmt; *p; p++) {
        if(*p != '%') {
            put_terminal_char(*p);
            continue;
        }
        switch (*++p) {
            case 'c':
                sval = (char*)(va_arg(ap, int) & ~0xFFFFFF00);
                put_terminal_char((int)sval);
                break;
            case 'd':
                ival = va_arg(ap, int);
                printf("%f", ival );
                break;
            case 'f':
                dval  = va_arg(ap, double);
                printf("%f", dval);
                break;
            case 's':
                for(sval = va_arg(ap, char *); *sval; sval++)
                    put_terminal_char(*sval);
                break;
            default:
                put_terminal_char(*p);
                break;
        }
    }
    if(NEW_LINE_AT_END)
        //Add new line
        put_terminal_char('\n');
    va_end(ap); 
}

#endif
