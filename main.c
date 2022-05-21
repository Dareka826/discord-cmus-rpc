#include "discord-rpc/include/discord_rpc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <math.h>
#include <assert.h>

#ifndef MEM_INFO_D
#define MEM_INFO_D 0
#endif

#define MIN(a,b) (((a) < (b)) ? (a) : (b))
#define MAX(a,b) (((a) > (b)) ? (a) : (b))

enum {
    PIPE_READ = 0,
    PIPE_WRITE
};

// Colors {{{
const char *colors[] = {
    "\033[0m",  // Clear
    "\033[31m", // Red
    "\033[32m", // Green
    "\033[33m", // Yellow
    "\033[34m", // Blue
    "\033[35m", // Magenta
    "\033[36m", // Cyan
    "\033[37m", // White/Gray
};

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

#define _C(NAME) colors[COLOR_ ## NAME] /*}}}*/

// Memory utility functions {{{
void * xmallocn(size_t s, const char * const name) {
    void *p = malloc(s);

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
}

void * xmalloc(size_t s) {
    return xmallocn(s, NULL);
}

void _nfreen(void *p, const char * const name) {
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
}

void _nfree(void *p) {
    _nfreen(p, NULL);
}

#define nfree(ptr)       { _nfree(ptr);        ptr = NULL; }
#define nfreen(ptr,name) { _nfreen(ptr, name); ptr = NULL; } /*}}}*/

// DISCORD {{{
const char * const APP_ID = "976582643938377749";

struct presence_state {
    char *state;
    char *details;
    int time_left;
};

void free_presence_state(struct presence_state *ps) { /*{{{*/
    if(MEM_INFO_D) fprintf(stderr, "[I]: Freeing presence_state\n");
    if(ps->state)   nfreen(ps->state,   "presence_state state");
    if(ps->details) nfreen(ps->details, "presence_state details");
    if(MEM_INFO_D) fprintf(stderr, "[I]: Finished freeing presence_state\n");
} /*}}}*/

// Functions to notify of events {{{
void discord_on_ready(const DiscordUser *connected_user) {
    fprintf(stderr, "[I]: Discord ready!\n");
}

void discord_on_error(int error_code, const char *message) {
    fprintf(stderr, "%s[E]:%s Discord error!\n", _C(ERROR), _C(CLEAR));
}

void discord_on_disconnected(int error_code, const char *message) {
    fprintf(stderr, "%s[E]%s: Discord disconnected!\n", _C(ERROR), _C(CLEAR));
} /*}}}*/

// Initialize rpc connection {{{
void discord_init() {
    DiscordEventHandlers handlers;
    memset(&handlers, 0, sizeof(handlers));

    handlers.ready        = discord_on_ready;
    handlers.errored      = discord_on_error;
    handlers.disconnected = discord_on_disconnected;

    Discord_Initialize(APP_ID, &handlers, 1, NULL);
} /*}}}*/

void (*discord_shutdown)(void) = Discord_Shutdown;

// Queues up an updare to rpc status (one change per 15 secs)
void discord_update_presence(struct presence_state *ps) { /*{{{*/
    DiscordRichPresence presence;
    memset(&presence, 0, sizeof(presence));

    presence.instance = 1;

    presence.largeImageKey = "cmus";
    presence.largeImageText = "C* Music Player";

    presence.state = ps->state;
    presence.details = ps->details;
    //presence.endTimestamp = time(NULL) + ps->time_left;

    Discord_UpdatePresence(&presence);
} /*}}}*/

void (*discord_clear_presence)(void) = Discord_ClearPresence;
// }}}

// CMUS {{{
struct cmus_state { /*{{{*/
    int status; // 1 is playing
    unsigned int position;
    unsigned int duration;

    char *title;
    char *file;
    char *artist;
    int tracknumber;

    char *album;
    char *albumartist;
}; /*}}}*/

void free_cmus_state(struct cmus_state *c) { /*{{{*/
    if(MEM_INFO_D) fprintf(stderr, "[I]: Freeing cmus_state\n");
    if(c->title)       nfreen(c->title,       "cmus_state title");
    if(c->file)        nfreen(c->file,        "cmus_state file");
    if(c->artist)      nfreen(c->artist,      "cmus_state artist");
    if(c->album)       nfreen(c->album,       "cmus_state album");
    if(c->albumartist) nfreen(c->albumartist, "cmus_state album_artist");
    if(MEM_INFO_D) fprintf(stderr, "[I]: Finished freeing cmus_state\n");
} /*}}}*/

/* Example Data {{{
status paused
file /home/rin/Music/[SPACELECTRO] VOCALO EDM10 HEAVY/01. Spacelectro - Reincarnation feat. Hatsune Miku.opus
duration 298
position 0
tag album VOCALO EDM10 HEAVY
tag albumartist SPACELECTRO
tag artist Spacelectro
tag title Reincarnation feat. Hatsune Miku
tag tracknumber 1
}}} */
void cmus_get_metadata(struct cmus_state * const cs) { /*{{{*/
    assert(cs != NULL);

    // Spawn cmus-remote and parse its output
    int pipefd[2];
    pipe(pipefd);

    pid_t pid = fork();
    if(pid == -1) exit(EXIT_FAILURE);

    if(pid == 0) {
        // Child
        close(pipefd[PIPE_READ]);
        dup2(pipefd[PIPE_WRITE], STDOUT_FILENO); // Redirect stdout to pipe
        dup2(STDOUT_FILENO, STDERR_FILENO); // Redirect stderr to stdout

        char *args[] = {"cmus-remote", "-Q", NULL};
        execvp("cmus-remote", args);

    } else {
        // Parent
        close(pipefd[PIPE_WRITE]);
        FILE *child = fdopen(pipefd[PIPE_READ], "r");

        char *line = NULL; // Line string
        size_t len = 0;
        size_t read_len = 0; // Length of read-in line

        if(MEM_INFO_D) fprintf(stderr, "[I]: Parsing cmus metadata\n");
        while((read_len = getline(&line, &len, child)) != -1) {
            if(MEM_INFO_D == 1)
                fprintf(stderr, "[I]: %s[MEM]%s getline() allocated %lu bytes at %p\n",
                        _C(MEM), _C(CLEAR), len, line);

            // Get rid of newline
            if(line[read_len-1] == '\n')
                line[read_len-1] = '\0';

            // Check if cmus is running
            if(strncmp(line, "cmus-remote: cmus is not running", 32) == 0) {
                fprintf(stderr, "[I]: cmus not running\n");
                cs->status = 0;
                nfreen(line, "line: cmus not running");
                break;
            }

            // Tags
            else if(strncmp(line, "tag ", 4) == 0) {
                // Title
                if(strncmp(line+4, "title ", 6) == 0) {
                    int len = strnlen(line+10, 4096) + 1;
                    cs->title = (char*) xmallocn(len, "parsed title");
                    memset(cs->title, 0, len);

                    strncpy(cs->title, line+10, len);
                }

                // Artist
                else if(strncmp(line+4, "artist ", 7) == 0) {
                    int len = strnlen(line+11, 4096) + 1;
                    cs->artist = (char*) xmallocn(len, "parsed artist");
                    memset(cs->artist, 0, len);

                    strncpy(cs->artist, line+11, len);
                }

                // Track
                else if(strncmp(line+4, "tracknumber ", 12) == 0) {
                    long num = strtol(line+16, NULL, 10);
                    cs->tracknumber = num;
                }

                // Album
                else if(strncmp(line+4, "album ", 6) == 0) {
                    int len = strnlen(line+10, 4096) + 1;
                    cs->album = (char*) xmallocn(len, "parsed album");
                    memset(cs->album, 0, len);

                    strncpy(cs->album, line+10, len);
                }

                // Album Artist
                else if(strncmp(line+4, "albumartist ", 12) == 0) {
                    int len = strnlen(line+16, 4096) + 1;
                    cs->albumartist = (char*) xmallocn(len, "parsed album_artist");
                    memset(cs->albumartist, 0, len);

                    strncpy(cs->albumartist, line+16, len);
                }
            }

            // Are we playing?
            else if(strncmp(line, "status ", 7) == 0) {
                if(strncmp(line+7, "playing", 7) == 0)
                     cs->status = 1;
                else cs->status = 0;
            }

            // Where we are in the track
            else if(strncmp(line, "position ", 9) == 0) {
                long num = strtol(line+9, NULL, 10);
                cs->position = num;
            }

            // Track length
            else if(strncmp(line, "duration ", 9) == 0) {
                long num = strtol(line+9, NULL, 10);
                cs->duration = num;
            }

            // Which file is being played
            else if(strncmp(line, "file ", 5) == 0) {
                int len = strnlen(line+5, 4096) + 1;
                cs->file = (char*) xmallocn(len, "parsed file");
                memset(cs->file, 0, len);

                strncpy(cs->file, line+5, len);
            }

            // Free memory allocated by getline()
            nfreen(line, "line: end of loop");
        }
        if(line != NULL) // Clean up after getline
            nfreen(line, "getline cleanup");

        if(MEM_INFO_D) fprintf(stderr, "[I]: Finished parsing cmus metadata\n");
    }
} /*}}}*/


void create_status(struct cmus_state const * const cs, /*{{{*/
                   struct presence_state * const ps) {

    // title?
    //   artist?
    //     tracknum?
    //       state = track. artist - title
    //     else
    //       state = artist - title
    //   else
    //     tracknum?
    //       state = track. title
    //     else
    //       state = title
    // else
    //   state = everything after last / in file
    //
    // album?
    //   album_artist?
    //     details = "[album_artist] album"
    //   else
    //     details = "album"
    // else
    //   details = ""

    if(MEM_INFO_D) fprintf(stderr, "[I]: Creating status from metadata\n");

    int state_len = 0;

    if(cs->title) {
        state_len += strnlen(cs->title, 512);

        if(cs->artist) {
            state_len += strnlen(cs->artist, 512);

            if(cs->tracknumber >= 1) {
                // track. artist - title
                state_len += floor(log10(abs(cs->tracknumber))) + 1;
                if(cs->tracknumber < 10) state_len++;

                state_len += 2 + 3; // ". ",  " - "
                ps->state = (char*) xmallocn(state_len+1, "create state");
                snprintf(ps->state, state_len+1, "%02d. %s - %s", cs->tracknumber, cs->artist, cs->title);
            } else {
                // artist - title

                state_len += 3; // " - "
                ps->state = (char*) xmallocn(state_len+1, "create state");
                snprintf(ps->state, state_len+1, "%s - %s", cs->artist, cs->title);
            }
        } else {
            if(cs->tracknumber >= 1) {
                // track. title
                state_len += floor(log10(abs(cs->tracknumber))) + 1;
                if(cs->tracknumber < 10) state_len++;

                state_len += 2; // ". "
                ps->state = (char*) xmallocn(state_len+1, "create state");
                snprintf(ps->state, state_len+1, "%02d. %s", cs->tracknumber, cs->title);
            } else {
                // title

                ps->state = (char*) xmallocn(state_len+1, "create state");
                snprintf(ps->state, state_len+1, "%s", cs->title);
            }
        }
    } else {
        // file
        ps->state = "<Not implemented>";
    }

    int details_len = 0;

    if(cs->album) {
        details_len += strnlen(cs->album, 512);

        if(cs->albumartist) {
            // [album_artist] album
            details_len += strnlen(cs->albumartist, 512);

            details_len += 3; // "[", "] "
            ps->details = (char*) xmallocn(details_len+1, "create details");
            snprintf(ps->details, details_len+1, "[%s] %s", cs->albumartist, cs->album);
        } else {
            // album

            ps->details = (char*) xmallocn(details_len+1, "create details");
            snprintf(ps->details, details_len+1, "%s", cs->album);
        }
    } else {
        // empty
        ps->details = "";
    }

    ps->time_left = cs->duration - cs->position;

    if(MEM_INFO_D) fprintf(stderr, "[I]: Finished creating status from metadata\n");
} /*}}}*/
// }}}

int main() {
    // TODO:
    // - change status on pause/play
    // - change song if metadata changes
    // - free stuff on SIGTERM

    // - if status != "playing", stop displaying rpc info
    // - if song data suddenly changes (check every 1 sec), reset rpc data

    discord_init();

    struct cmus_state cs;
    struct presence_state ps;

    int was_playing = 0;
    struct presence_state old_ps;
    memset(&old_ps, 0, sizeof(old_ps));
    old_ps.state = (char*) malloc(1);
    old_ps.state[0] = '\0';
    old_ps.details = (char*) malloc(1);
    old_ps.details[0] = '\0';

    // Main logic {{{
    while(1) {
        if(MEM_INFO_D) fprintf(stderr, "\n");
        memset(&cs, 0, sizeof(cs));
        cmus_get_metadata(&cs);

        if(cs.status == 1) {
            create_status(&cs, &ps);

            int changed = 0;
            {
                int old_state_len = strnlen(old_ps.state, 512);
                int new_state_len = strnlen(    ps.state, 512);
                int old_details_len = strnlen(old_ps.details, 512);
                int new_details_len = strnlen(    ps.details, 512);

                if( (was_playing == 0) ||
                    (old_state_len   != new_state_len)   ||
                    (old_details_len != new_details_len) ||
                    (strncmp(old_ps.state,   ps.state,   MAX(old_state_len,   new_state_len))   != 0) ||
                    (strncmp(old_ps.details, ps.details, MAX(old_details_len, new_details_len)) != 0) ) {

                    changed = 1;
                }
            }

            if(changed != 0) {
                printf("[I]: Changed!\n");

                printf("State: %s\n", ps.state);
                printf("Deets: %s\n", ps.details);
                //printf("Timel: %d\n", ps.time_left);

                discord_update_presence(&ps);
            }

            was_playing = 1;

            free_presence_state(&old_ps);
            memcpy(&old_ps, &ps, sizeof(struct presence_state));

        } else {
            if(was_playing == 1) {
                printf("[I]: Not playing\n");
                was_playing = 0;

                discord_clear_presence();
            }
        }

        free_cmus_state(&cs);
        if(MEM_INFO_D) fprintf(stderr, "\n");
        sleep(1);
    } /*}}}*/

    discord_shutdown();
    return EXIT_SUCCESS;
}
