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

int memcmp(uint8_t* first, uint8_t* second, int n)
{
    while(n--)
    {
        if(*first != *second)
            return 0;
        first += 1;
        second += 1;
    }
}

void memset(char *dst,  char *value, int n)
{
    char *tmp = dst;
    for(;n != 0; n--)
    {
        *tmp++ = *value;
    }
}

int strcpy(char *destination, const char* source)
{
    int i = 0;
    while((*destination++ = *source++) != 0)
        i++;
    return i;
}

int strcmp(const char *destination, char *source)
{
    int i = 0;
    while((destination[i] == source[i]))
    {
        if(source[i++] == 0)
            return 0;
    }
    return 1;
}

char *separate_string(char **string, const char* delimiter)
{
    char *s;
    const char *span;
    int count, sub_count;
    char *token;
    if((s = *string) == NULL)
    {
        return NULL;
    }
    for(token = s;;)
    {
        count = *s++;
        span = delimiter;
        do
        {
            if((sub_count = *span++) == count)
            {
                if(count == 0)
                    s = NULL;
                else
                    s[-1] = 0;
                string = s;
                return token;
            }
        } while ( sub_count != 0 );
        
    }
}

#endif