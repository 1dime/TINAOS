#ifndef STDIO_C
#define STDIO_C
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>


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
    va_end(ap); 
}


#endif