#ifndef __COLORS_H
#define __COLORS_H

extern const char *colors[];

enum {
    COLOR_CLEAR = 0,
    COLOR_RED,
    COLOR_GREEN,
    COLOR_YELLOW,
    COLOR_BLUE,
    COLOR_MAGENTA,
    COLOR_CYAN,
    COLOR_WHITE,

    COLOR_ERROR = COLOR_RED,
    COLOR_WARN = COLOR_YELLOW,
    COLOR_NULLFREE = COLOR_MAGENTA,
    COLOR_MEM = COLOR_BLUE,
    COLOR_NAME = COLOR_CYAN
};

#define _C(NAME) colors[COLOR_ ## NAME]

#endif
