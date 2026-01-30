// kernel/include/string.h
#ifndef STRING_H
#define STRING_H

#include "types.h"

// Calculate string length.
size_t strlen(const char* str);

// Compare two strings.
int strcmp(const char* s1, const char* s2);

// Compare n characters of two strings.
int strncmp(const char* s1, const char* s2, size_t n);

// Copy string.
char* strcpy(char* dest, const char* src);

// Copy n characters of string.
char* strncpy(char* dest, const char* src, size_t n);

// Concatenate strings.
char* strcat(char* dest, const char* src);

// Convert integer to string.
void itoa(int value, char* str, int base);

#endif // STRING_H
