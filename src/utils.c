#include "utils.h"

#include <stdio.h>
#include "colors.h"

void * xmallocn(size_t s, const char * const name) { /*{{{*/
    void * const p = malloc(s);

    if(p == NULL) {
        fprintf(stderr, "%s[E]%s: malloc() failed, exiting...\n",
                _C(ERROR), _C(CLEAR));
        exit(1);
    }

    if(MEM_INFO_D == 1) {
        if(name != NULL)
             fprintf(stderr, "[I]: %s[MEM]%s allocated %lu bytes at %p (%s%s%s)\n",
                     _C(MEM), _C(CLEAR), s, p, _C(NAME), name, _C(CLEAR));
        else fprintf(stderr, "[I]: %s[MEM]%s allocated %lu bytes at %p\n",
                     _C(MEM), _C(CLEAR), s, p);
    }

    return p;
} /*}}}*/

void * xmalloc(size_t s) {
    return xmallocn(s, NULL);
}

void * xcallocn(size_t s, const char * const name) { /*{{{*/
    void * const p = calloc(1, s);

    if(p == NULL) {
        fprintf(stderr, "%s[E]%s: calloc() failed, exiting...\n",
                _C(ERROR), _C(CLEAR));
        exit(1);
    }

    if(MEM_INFO_D == 1) {
        if(name != NULL)
             fprintf(stderr, "[I]: %s[MEM]%s allocated %lu bytes at %p (%s%s%s)\n",
                     _C(MEM), _C(CLEAR), s, p, _C(NAME), name, _C(CLEAR));
        else fprintf(stderr, "[I]: %s[MEM]%s allocated %lu bytes at %p\n",
                     _C(MEM), _C(CLEAR), s, p);
    }

    return p;
} /*}}}*/

void * xcalloc(size_t s) {
    return xcallocn(s, NULL);
}

void _nfreen(void * const p, const char * const name) { /*{{{*/
    if(MEM_INFO_D == 1) {
        if(p != NULL) {
            if(name != NULL)
                 fprintf(stderr, "[I]: %s[MEM]%s freeing %p (%s%s%s)\n",
                         _C(MEM), _C(CLEAR), p, _C(NAME), name, _C(CLEAR));
            else fprintf(stderr, "[I]: %s[MEM]%s freeing %p\n",
                         _C(MEM), _C(CLEAR), p);
        }
    }

    if(p != NULL) free(p);
    else {
        if(MEM_INFO_D == 1 && name != NULL)
             fprintf(stderr, "%s[W]%s: %s[MEM]%s %sTried to free NULL!%s (%s%s%s)\n",
                     _C(WARN), _C(CLEAR), _C(MEM), _C(CLEAR),
                     _C(NULLFREE), _C(CLEAR), _C(NAME), name, _C(CLEAR));
        else fprintf(stderr, "%s[W]%s: %s[MEM]%s %sTried to free NULL!%s\n",
                     _C(WARN), _C(CLEAR), _C(MEM), _C(CLEAR),
                     _C(NULLFREE), _C(CLEAR));
    }
} /*}}}*/

void _nfree(void * const p) {
    _nfreen(p, NULL);
}
