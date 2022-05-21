#include "../src/discord_stuff.h"

#include "../src/utils.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    // Initialize presence_state
    struct presence_state ps;
    memset(&ps, 0, sizeof(ps));

    assert(ps.state     == NULL);
    assert(ps.details   == NULL);
    assert(ps.time_left == 0);

    const char * str = "Test1";
    ps.state = (char*) xmalloc(strlen(str)+1);
    memcpy(ps.state, str, strlen(str)+1);

    const char * str2 = "Another string!";
    ps.details = (char*) xmalloc(strlen(str2)+1);
    memcpy(ps.details, str2, strlen(str2)+1);

    assert(ps.state   != NULL);
    assert(ps.details != NULL);

    assert(strcmp(ps.state,   str ) == 0);
    assert(strcmp(ps.details, str2) == 0);

    // Freeing
    free_presence_state(&ps);

    assert(ps.state   == NULL);
    assert(ps.details == NULL);

    return EXIT_SUCCESS;
}
