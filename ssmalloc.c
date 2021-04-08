#include "ssmalloc.h"

#include <pthread.h>
#include <stddef.h>

/*
 * Total size of memory available for allocation with ssmalloc().
 */
#ifndef SSMALLOC_ARENA_SIZE
#define SSMALLOC_ARENA_SIZE 1048576 /* 1 MB */
#endif

/*
 * Size of a memory chunk allocated with ssmalloc().
 */
#ifndef SSMALLOC_CHUNK_SIZE
#define SSMALLOC_CHUNK_SIZE 128
#endif

#if SSMALLOC_CHUNK_SIZE > SSMALLOC_ARENA_SIZE
#error "SSMALLOC_CHUNK_SIZE must be <= SSMALLOC_ARENA_SIZE"
#endif

#if SSMALLOC_CHUNK_SIZE < __SIZEOF_POINTER__
#error "SSMALLOC_CHUNK_SIZE must be >= __SIZEOF_POINTER__"
#endif

/*
 * Static memory area used for allocations. Free chunks are linked in a singly
 * linked list with the free_ptr pointing to the head. A pointer to the next
 * free chunk (or NULL for the tail element) is stored in the first word of
 * each free chunk. Protected by arena_mutex.
 */
static char arena[SSMALLOC_ARENA_SIZE]
	__attribute__((aligned(__SIZEOF_POINTER__)));
static void *free_ptr;
static pthread_mutex_t arena_mutex = PTHREAD_MUTEX_INITIALIZER;

size_t ssmalloc_arena_size(void) { return SSMALLOC_ARENA_SIZE; }
size_t ssmalloc_chunk_size(void) { return SSMALLOC_CHUNK_SIZE; }

void *ssmalloc(void)
{
	void *ptr = NULL;
	pthread_mutex_lock(&arena_mutex);
	if (free_ptr) {
		ptr = free_ptr;
		free_ptr = *(void **)ptr;
	} else
	pthread_mutex_unlock(&arena_mutex);
	return ptr;
}

void ssmfree(void *ptr)
{
	if (!ptr)
		return;
	pthread_mutex_lock(&arena_mutex);
	*(void **)ptr = free_ptr;
	free_ptr = ptr;
	pthread_mutex_unlock(&arena_mutex);
}

__attribute__((constructor))
static void ssmalloc_init(void)
{
	int i;
	for (i = 0; i < SSMALLOC_ARENA_SIZE / SSMALLOC_CHUNK_SIZE - 1; i++) {
		*(void **)&arena[i * SSMALLOC_CHUNK_SIZE] =
			&arena[(i + 1) * SSMALLOC_CHUNK_SIZE];
	}
	*(void **)&arena[i * SSMALLOC_CHUNK_SIZE] = NULL;
	free_ptr = &arena[0];
}
