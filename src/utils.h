#ifndef __UTILS_H
#define __UTILS_H

#include <stdlib.h>

#ifdef MEM_INFO_D
#define _mem_info_log(fmt, ...) do { fprintf(stderr, fmt, ##__VA_ARGS__); } while (0)
#else
#define _mem_info_log(...) do { } while (0)
#endif

// Comparison
#define MIN(a,b) (((a) < (b)) ? (a) : (b))
#define MAX(a,b) (((a) > (b)) ? (a) : (b))

// Define pipe end meanings
enum {
    PIPE_READ = 0,
    PIPE_WRITE
};

// Memory utility functions

// Malloc that exits on error
void * xmallocn(size_t s, const char * const name);
void * xmalloc(size_t s);

// Calloc that exits on error
void * xcallocn(size_t s, const char * const name);
void * xcalloc(size_t s);

// Free that protects against freeing NULL and warns
void _nfreen(void * const p, const char * const name);
void _nfree(void * const p);

// Free that sets the freed ptr to NULL afterwards
#define nfree(ptr)        do { _nfree(ptr);        ptr = NULL; } while (0)
#define nfreen(ptr, name) do { _nfreen(ptr, name); ptr = NULL; } while (0)

// Quiet nfree (for stdlib functions that allocate memory)
#define nqfree(ptr) do { free(ptr); ptr = NULL; } while (0)

#endif
