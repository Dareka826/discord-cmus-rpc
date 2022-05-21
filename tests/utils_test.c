#include "../src/utils.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    // Comparison
    assert(MIN(43, 54) == 43);
    assert(MIN(54, 43) == 43);

    assert(MAX(43, 54) == 54);
    assert(MAX(54, 43) == 54);

    // Pipe
    assert(PIPE_READ >= 0);
    assert(PIPE_READ <= 1);
    assert(PIPE_WRITE >= 0);
    assert(PIPE_WRITE <= 1);

    // Memory utils
    char *p = (char*) xmalloc(10);
    assert(p != NULL && "xmalloc returned NULL"); // Should never happen (xmalloc would just exit)

    // Check that the allocated memory is ok
    memset(p, 0, 10);
    for(int i = 0; i < 10; i++)
        assert(p[i] == 0 && "memory didn't give back what was written");

    nfree(p);
    assert(p == NULL && "nfree didn't set ptr to NULL");

    return EXIT_SUCCESS;
}
