SSMALLOC_CHUNK_SIZE = 64
SSMALLOC_ARENA_SIZE = 4194304

CC = gcc
CFLAGS = -Wall -Werror

ssmalloc.a: ssmalloc.o
	ar rcs $@ $^

ssmalloc.o: ssmalloc.c ssmalloc.h Makefile
	$(CC) $(CFLAGS) \
		-DSSMALLOC_ARENA_SIZE=$(SSMALLOC_ARENA_SIZE) \
		-DSSMALLOC_CHUNK_SIZE=$(SSMALLOC_CHUNK_SIZE) \
		-c -lpthread -o $@ $<

ssmalloc_test: ssmalloc_test.c ssmalloc.a
	$(CC) $(CFLAGS) -o $@ $^

PHONY += test
test: ssmalloc_test
	./ssmalloc_test

PHONY += clean
clean:
	$(RM) ssmalloc.o ssmalloc.a ssmalloc_test

.PHONY: $(PHONY)
