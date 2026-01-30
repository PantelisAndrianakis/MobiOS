// kernel/include/types.h

#ifndef TYPES_H
#define TYPES_H

// Standard type definitions
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;

typedef signed char int8_t;
typedef signed short int16_t;
typedef signed int int32_t;
typedef signed long long int64_t;

typedef uint64_t size_t;
typedef int64_t ssize_t;

// Boolean type
#define bool unsigned char
#define false 0
#define true 1

// NULL pointer
#define NULL ((void*)0)

#endif // TYPES_H