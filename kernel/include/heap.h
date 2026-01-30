// kernel/include/heap.h
#ifndef HEAP_H
#define HEAP_H

#include "types.h"

// Allocate memory from heap.
void* kmalloc(size_t size);

// Free memory (not implemented - bump allocator).
void kfree(void* ptr);

#endif // HEAP_H
