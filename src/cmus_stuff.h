#ifndef __CMUS_STUFF_H
#define __CMUS_STUFF_H

#include "discord_stuff.h"
#include "snippets/arena/arena.h"

struct cmus_state {
    int status; // 1 is playing
    unsigned int position;
    unsigned int duration;

    char *title;
    char *file;
    char *artist;
    int tracknumber;

    char *album;
    char *albumartist;
};

// Parse the output of "cmus-remote -Q" into a cmus_state struct
void cmus_get_metadata(Arena * const ap, struct cmus_state * const cs);

// Create a formatted state out of data from cmus
void create_status(Arena * const ap, const struct cmus_state * const cs, struct presence_state * const ps);

// For internal use
void _cmus_parse_line(Arena * const ap, struct cmus_state * const cs, const char * const line);

#endif
