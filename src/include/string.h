#ifndef STRING
#define STRING
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>

size_t strlen(const char* str)
{
    size_t len = 0;
    while(str[len])
        len++;
    return len;
}

#endif