#include "../src/colors.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    // Check that colors correspond to the right color codes
    assert(strcmp(colors[COLOR_CLEAR],   "\033[0m" ) == 0);
    assert(strcmp(colors[COLOR_RED],     "\033[31m") == 0);
    assert(strcmp(colors[COLOR_GREEN],   "\033[32m") == 0);
    assert(strcmp(colors[COLOR_YELLOW],  "\033[33m") == 0);
    assert(strcmp(colors[COLOR_BLUE],    "\033[34m") == 0);
    assert(strcmp(colors[COLOR_MAGENTA], "\033[35m") == 0);
    assert(strcmp(colors[COLOR_CYAN],    "\033[36m") == 0);
    assert(strcmp(colors[COLOR_WHITE],   "\033[37m") == 0);

    // Check if the _C macro works
    assert(_C(CLEAR)   == colors[COLOR_CLEAR]);
    assert(_C(RED)     == colors[COLOR_RED]);
    assert(_C(GREEN)   == colors[COLOR_GREEN]);
    assert(_C(YELLOW)  == colors[COLOR_YELLOW]);
    assert(_C(BLUE)    == colors[COLOR_BLUE]);
    assert(_C(MAGENTA) == colors[COLOR_MAGENTA]);
    assert(_C(CYAN)    == colors[COLOR_CYAN]);
    assert(_C(WHITE)   == colors[COLOR_WHITE]);

    return EXIT_SUCCESS;
}
