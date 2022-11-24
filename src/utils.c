#include "utils.h"

#include <stdio.h>
#include <assert.h>
#include "colors.h"

void * xmallocn(size_t s, const char * const name) { /*{{{*/
    assert(s > 0);
    void * const p = malloc(s);

    if(p == NULL) {
        _mem_info_log("%s[E]%s: malloc() failed, exiting...\n",
                _C(ERROR), _C(CLEAR));
        exit(1);
    }

    #ifdef MEM_INFO_D
    if(name != NULL)
         _mem_info_log("[I]: %s[MEM]%s allocated %lu bytes at %p (%s%s%s)\n",
                 _C(MEM), _C(CLEAR), s, p, _C(NAME), name, _C(CLEAR));
    else _mem_info_log("[I]: %s[MEM]%s allocated %lu bytes at %p\n",
                 _C(MEM), _C(CLEAR), s, p);
    #endif

    return p;
} /*}}}*/

void * xmalloc(size_t s) {
    return xmallocn(s, NULL);
}

void * xcallocn(size_t s, const char * const name) { /*{{{*/
    assert(s > 0);
    void * const p = calloc(1, s);

    if(p == NULL) {
        _mem_info_log("%s[E]%s: calloc() failed, exiting...\n",
                _C(ERROR), _C(CLEAR));
        exit(1);
    }

    #ifdef MEM_INFO_D
    if(name != NULL)
         _mem_info_log("[I]: %s[MEM]%s allocated %lu bytes at %p (%s%s%s)\n",
                 _C(MEM), _C(CLEAR), s, p, _C(NAME), name, _C(CLEAR));
    else _mem_info_log("[I]: %s[MEM]%s allocated %lu bytes at %p\n",
                 _C(MEM), _C(CLEAR), s, p);
    #endif

    return p;
} /*}}}*/

void * xcalloc(size_t s) {
    return xcallocn(s, NULL);
}

void _nfreen(void * const p, const char * const name) { /*{{{*/
    #ifdef MEM_INFO_D
    if(p != NULL) {
        if(name != NULL)
             _mem_info_log("[I]: %s[MEM]%s freeing %p (%s%s%s)\n",
                     _C(MEM), _C(CLEAR), p, _C(NAME), name, _C(CLEAR));
        else _mem_info_log("[I]: %s[MEM]%s freeing %p\n",
                     _C(MEM), _C(CLEAR), p);
    }
    #endif

    if(p != NULL)
        free(p);
    #ifdef MEM_INFO_D
    else {
        if(name != NULL)
             _mem_info_log("%s[W]%s: %s[MEM]%s %sTried to free NULL!%s (%s%s%s)\n",
                     _C(WARN), _C(CLEAR), _C(MEM), _C(CLEAR),
                     _C(NULLFREE), _C(CLEAR), _C(NAME), name, _C(CLEAR));
        else _mem_info_log("%s[W]%s: %s[MEM]%s %sTried to free NULL!%s\n",
                     _C(WARN), _C(CLEAR), _C(MEM), _C(CLEAR),
                     _C(NULLFREE), _C(CLEAR));
    }
    #endif
} /*}}}*/

void _nfree(void * const p) {
    _nfreen(p, NULL);
}
