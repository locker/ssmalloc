#include "ssmalloc.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FAIL_IF(condition)						\
	if (condition) {						\
		fprintf(stderr, "FAIL: %s:%d: %s\n",			\
			__FILE__, __LINE__, #condition);		\
		exit(1);						\
	}

void test_alloc_free(void)
{
	void *p = ssmalloc();
	FAIL_IF(p == NULL);
	memset(p, 'a', ssmalloc_chunk_size());
	ssmfree(p);
}

void test_free_null(void)
{
	ssmfree(NULL);
	ssmfree(NULL);
}

void test_no_memory(void)
{
	int num_chunks = ssmalloc_arena_size() / ssmalloc_chunk_size();
	void **chunks = malloc(num_chunks * sizeof(*chunks));
	FAIL_IF(chunks == NULL);
	for (int i = 0; i < num_chunks; i++) {
		chunks[i] = ssmalloc();
		FAIL_IF(chunks[i] == NULL);
	}
	FAIL_IF(ssmalloc() != NULL);
	for (int i = 0; i < num_chunks; i++) {
		ssmfree(chunks[i]);
	}
}

int main()
{
	test_alloc_free();
	test_free_null();
	test_no_memory();
	puts("SUCCESS");
	return 0;
}
