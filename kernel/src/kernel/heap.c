// kernel/src/kernel/heap.c
#include "heap.h"
#include "kernel.h"

// Heap implementation.
static uint8_t heap[1024 * 1024];  // 1MB heap.
static size_t heap_offset = 0;

// Allocate memory from heap.
void* kmalloc(size_t size)
{
	if (heap_offset + size > sizeof(heap))
	{
		return NULL;
	}
	
	void* ptr = &heap[heap_offset];
	heap_offset += size;
	return ptr;
}

// Free memory (not implemented - bump allocator).
void kfree(void* ptr)
{
	// Do nothing for now.
	(void)ptr;
}
