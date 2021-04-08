#ifndef SSMALLOC_H
#define SSMALLOC_H

#include <stddef.h>

/*
 * Returns the total size of the memory area used for allocations.
 */
size_t ssmalloc_arena_size(void);

/*
 * Returns the size of memory chunks allocated by ssmalloc().
 */
size_t ssmalloc_chunk_size(void);

/*
 * Allocates a chunk of memory of size ssmalloc_size() and returns a pointer to
 * it. Returns NULL if there is no free chunks left. Thread-safe.
 */
void *ssmalloc(void);

/*
 * Frees a memory chunk previously allocated with ssmalloc(). If ptr is NULL,
 * this function does nothing. Thread-safe.
 */
void ssmfree(void *ptr);

#endif /* SSMALLOC_H */
