// kernel/src/kernel/string.c
#include "string.h"
#include "kernel.h"

// Calculate string length.
size_t strlen(const char* str)
{
	size_t len = 0;
	while (str[len])
	{
		len++;
	}
	return len;
}

// Compare two strings.
int strcmp(const char* s1, const char* s2)
{
	while (*s1 && (*s1 == *s2))
	{
		s1++;
		s2++;
	}
	return *(unsigned char*)s1 - *(unsigned char*)s2;
}

// Compare n characters of two strings.
int strncmp(const char* s1, const char* s2, size_t n)
{
	while (n && *s1 && (*s1 == *s2))
	{
		s1++;
		s2++;
		n--;
	}
	if (n == 0)
	{
		return 0;
	}
	return *(unsigned char*)s1 - *(unsigned char*)s2;
}

// Copy string.
char* strcpy(char* dest, const char* src)
{
	char* orig_dest = dest;
	while (*src)
	{
		*dest++ = *src++;
	}
	*dest = '\0';
	return orig_dest;
}

// Copy n characters of string.
char* strncpy(char* dest, const char* src, size_t n)
{
	size_t i;
	for (i = 0; i < n && src[i] != '\0'; i++)
	{
		dest[i] = src[i];
	}
	for (; i < n; i++)
	{
		dest[i] = '\0';
	}
	return dest;
}

// Concatenate strings.
char* strcat(char* dest, const char* src)
{
	char* orig_dest = dest;
	while (*dest)
	{
		dest++;
	}
	while (*src)
	{
		*dest++ = *src++;
	}
	*dest = '\0';
	return orig_dest;
}

// Convert integer to string.
void itoa(int value, char* str, int base)
{
	char* ptr = str;
	char* ptr1 = str;
	char tmp_char;
	int tmp_value;
	
	// Handle negative numbers for base 10.
	if (value < 0 && base == 10)
	{
		*ptr++ = '-';
		value = -value;
		ptr1++;
	}
	
	// Convert to string (reversed).
	do
	{
		tmp_value = value;
		value /= base;
		*ptr++ = "0123456789abcdef"[tmp_value - value * base];
	}
	while (value);
	
	*ptr-- = '\0';
	
	// Reverse string.
	while (ptr1 < ptr)
	{
		tmp_char = *ptr;
		*ptr-- = *ptr1;
		*ptr1++ = tmp_char;
	}
}
