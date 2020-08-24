#ifndef STRING
#define STRING
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>
#include "../../sys/malloc.c"

size_t strlen(const char* str);
void* memcpy(void *destination, void const *src, int number);
int memcmp(uint8_t* first, uint8_t* second, int n);

void memset(char *dst,  char *value, int n);

int strcpy(char *destination, const char* source);

int strcmp(const char *destination, char *source);

char *strsep(char **string, const char* delimiter);

char *strdup(const char *source);
#endif