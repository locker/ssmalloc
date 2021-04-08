ssmalloc
========

This repository contains a trivial memory allocation library. The library
provides two methods `ssmalloc()` and `ssmfree()`, which can be used for
allocating/freeing fixed-size memory blocks from a statically allocated memory
pool. The size of a memory block as well as the size of the pool are defined at
compile-time with `SSMALLOC_CHUNK_SIZE` and `SSMALLOC_ARENA_SIZE` macro
definitions. The library is thread-safe (it uses a pthread mutex to protect the
pool).

The library was written solely for educational purposes and is not supposed to
be used at all.

Usage
-----

To build the library (`ssmalloc.a`), invoke `make`.

To run the tests, invoke `make test`.
