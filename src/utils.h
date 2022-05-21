#ifndef __UTILS_H
#define __UTILS_H

#include <stdlib.h>

#ifndef MEM_INFO_D
#define MEM_INFO_D 0
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

// Free that protects against freeing NULL and warns
void _nfreen(void *p, const char * const name);
void _nfree(void *p);

// Free that sets the freed ptr to NULL afterwards
#define nfree(ptr)       { _nfree(ptr);        ptr = NULL; }
#define nfreen(ptr,name) { _nfreen(ptr, name); ptr = NULL; }

#endif
