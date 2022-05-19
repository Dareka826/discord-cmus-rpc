#include "discord-rpc/include/discord_rpc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

enum {
    PIPE_READ = 0,
    PIPE_WRITE
};

// DISCORD {{{
const char * const APP_ID = "976582643938377749";

// Functions to notify of events {{{
void discord_on_ready(const DiscordUser *connected_user) {
    fprintf(stderr, "[I]: Discord ready!\n");
}

void discord_on_error(int error_code, const char *message) {
    fprintf(stderr, "[E]: Discord error!\n");
}

void discord_on_disconnected(int error_code, const char *message) {
    fprintf(stderr, "[E]: Discord disconnected!\n");
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
void discord_update_presence() { /*{{{*/
    DiscordRichPresence presence;
    memset(&presence, 0, sizeof(presence));

    presence.instance = 1;

    presence.largeImageKey = "cmus";
    presence.largeImageText = "C* Music Player";

    presence.state = "State";
    presence.details = "Details";

    presence.endTimestamp = time(NULL) + 10; // 10 seconds from now

    Discord_UpdatePresence(&presence);
} /*}}}*/
// }}}

// CMUS {{{
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

/*
status paused
file /home/rin/Music/[SPACELECTRO] VOCALO EDM10 HEAVY/01. Spacelectro - Reincarnation feat. Hatsune Miku.opus
duration 298
position 0
tag album VOCALO EDM10 HEAVY
tag albumartist SPACELECTRO
tag artist Spacelectro
tag title Reincarnation feat. Hatsune Miku
tag tracknumber 1
*/
void cmus_get_metadata(struct cmus_state *c) { /*{{{*/
    // Spawn cmus-remote and parse its output
    int pipefd[2];
    pipe(pipefd);

    pid_t pid = fork();
    if(pid == -1) exit(EXIT_FAILURE);

    if(pid == 0) {
        // Child
        close(pipefd[PIPE_READ]);
        dup2(STDOUT_FILENO, STDERR_FILENO); // Redirect stderr to stdout
        dup2(pipefd[PIPE_WRITE], STDOUT_FILENO); // Redirect stdout to pipe

        char *args[] = {"cmus-remote", "-Q", NULL};
        execvp("cmus-remote", args);

    } else {
        // Parent
        close(pipefd[PIPE_WRITE]);
        FILE *child = fdopen(pipefd[PIPE_READ], "r");

        char *line = NULL; // Line string
        size_t len = 0;
        size_t read_len = 0; // Length of read-in line

        struct cmus_state cs;
        memset(&cs, 0, sizeof(cs));
        cs.tracknumber = -1;

        while((read_len = getline(&line, &len, child)) != -1) {

            // Get rid of newline
            if(line[read_len-1] == '\n')
                line[read_len-1] = '\0';

            // Check if cmus is running
            if(strncmp(line, "cmus-remote: cmus is not running", 32) == 0) {
                cs.status = 0;
                break;
            }

            // Tags
            else if(strncmp(line, "tag ", 4) == 0) {
                // Title
                if(strncmp(line+4, "title ", 6) == 0) {
                    int len = strnlen(line+10, 4096) + 1;
                    cs.title = (char*) malloc(len);
                    memset(cs.title, 0, len);

                    strncpy(cs.title, line+10, len);
                }

                // Artist
                else if(strncmp(line+4, "artist ", 7) == 0) {
                    int len = strnlen(line+11, 4096) + 1;
                    cs.artist = (char*) malloc(len);
                    memset(cs.artist, 0, len);

                    strncpy(cs.artist, line+11, len);
                }

                // Track
                else if(strncmp(line+4, "tracknumber ", 12) == 0) {
                    long num = strtol(line+16, NULL, 10);
                    cs.tracknumber = num;
                }

                // Album
                else if(strncmp(line+4, "album ", 6) == 0) {
                    int len = strnlen(line+10, 4096) + 1;
                    cs.album = (char*) malloc(len);
                    memset(cs.album, 0, len);

                    strncpy(cs.album, line+10, len);
                }

                // Album Artist
                else if(strncmp(line+4, "albumartist ", 12) == 0) {
                    int len = strnlen(line+16, 4096) + 1;
                    cs.albumartist = (char*) malloc(len);
                    memset(cs.albumartist, 0, len);

                    strncpy(cs.albumartist, line+16, len);
                }
            }

            // Are we playing?
            else if(strncmp(line, "status ", 7) == 0) {
                if(strncmp(line+7, "playing", 7) == 0)
                     cs.status = 1;
                else cs.status = 0;
            }

            // Where we are in the track
            else if(strncmp(line, "position ", 9) == 0) {
                long num = strtol(line+9, NULL, 10);
                cs.position = num;
            }

            // Track length
            else if(strncmp(line, "duration ", 9) == 0) {
                long num = strtol(line+9, NULL, 10);
                cs.duration = num;
            }

            // Which file is being played
            else if(strncmp(line, "file ", 5) == 0) {
                int len = strnlen(line+5, 4096) + 1;
                cs.file = (char*) malloc(len);
                memset(cs.file, 0, len);

                strncpy(cs.file, line+5, len);
            }

            // Free memory allocated by getline()
            free(line); line = NULL;
        }

        // Copy gathered data into giver struct pointer
        // TODO: work on the pointer without a local copy
        memcpy(c, &cs, sizeof(cs));
    }
} /*}}}*/

// }}}

int main() {
    //discord_init();

    struct cmus_state cs;
    cmus_get_metadata(&cs);

    printf("status: %d\n", cs.status);
    printf("position: %d\n", cs.position);
    printf("duration: %d\n", cs.duration);
    printf("title: %s\n", cs.title);
    printf("file: %s\n", cs.file);
    printf("artist: %s\n", cs.artist);
    printf("track: %d\n", cs.tracknumber);
    printf("album: %s\n", cs.album);
    printf("album_artist: %s\n", cs.albumartist);

    //discord_update_presence();
    //sleep(10);

    //discord_shutdown();
    return EXIT_SUCCESS;
}
