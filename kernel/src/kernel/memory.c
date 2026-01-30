// kernel/src/kernel/memory.c

#include "kernel.h"

// Copy memory from source to destination
void* memcpy(void* dest, const void* src, size_t count)
{
    char* d = (char*)dest;
    const char* s = (const char*)src;
    
    for (size_t i = 0; i < count; i++)
    {
        d[i] = s[i];
    }
    
    return dest;
}

// Fill memory with a specific value
void* memset(void* dest, int val, size_t count)
{
    unsigned char* d = (unsigned char*)dest;
    unsigned char value = (unsigned char)val;
    
    for (size_t i = 0; i < count; i++)
    {
        d[i] = value;
    }
    
    return dest;
}