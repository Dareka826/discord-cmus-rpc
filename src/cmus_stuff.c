#include "cmus_stuff.h"

#include "colors.h"
#include "utils.h"
#include "discord_stuff.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <math.h>
#include <assert.h>

void free_cmus_state(struct cmus_state * const c) { /*{{{*/
    assert(c != NULL);
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
void _cmus_parse_line(struct cmus_state * const cs, const char * const line) { /*{{{*/
    assert(cs != NULL);

    // Check if cmus is running
    if(strncmp(line, "cmus-remote: cmus is not running", 32) == 0) {
        cs->status = 0;
        return;
    }

    // Tags
    if(strncmp(line, "tag ", 4) == 0) {
        // Title
        if(strncmp(line+4, "title ", 6) == 0) {
            int len = strnlen(line+10, 4096) + 1;
            cs->title = (char*) xcallocn(len, "parsed title");

            strncpy(cs->title, line+10, len);
        }

        // Artist
        else if(strncmp(line+4, "artist ", 7) == 0) {
            int len = strnlen(line+11, 4096) + 1;
            cs->artist = (char*) xcallocn(len, "parsed artist");

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
            cs->album = (char*) xcallocn(len, "parsed album");

            strncpy(cs->album, line+10, len);
        }

        // Album Artist
        else if(strncmp(line+4, "albumartist ", 12) == 0) {
            int len = strnlen(line+16, 4096) + 1;
            cs->albumartist = (char*) xcallocn(len, "parsed album_artist");

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
        cs->file = (char*) xcallocn(len, "parsed file");

        strncpy(cs->file, line+5, len);
    }
} /*}}}*/

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
        signal(SIGCHLD, SIG_IGN); // Ignore child exit (prevent zombie processes)
        FILE *child = fdopen(pipefd[PIPE_READ], "r");

        char *line = NULL; // Line string
        size_t len = 0;
        size_t read_len = 0; // Length of read-in line

        if(MEM_INFO_D) fprintf(stderr, "[I]: Parsing cmus metadata\n");

        cs->status = -1; // For detecting errors and paused playback

        // Read each line of output from child
        while((read_len = getline(&line, &len, child)) != -1) {
            if(MEM_INFO_D == 1)
                fprintf(stderr, "[I]: %s[MEM]%s getline() allocated %lu bytes at %p\n",
                        _C(MEM), _C(CLEAR), len, line);

            // Get rid of newline
            if(line[read_len-1] == '\n')
                line[read_len-1] = '\0';

            _cmus_parse_line(cs, line);
            if(cs->status == 0) break; // Not playing

            // Free memory allocated by getline()
            nfreen(line, "line: end of loop");
        }
        if(line != NULL) // Clean up after getline
            nfreen(line, "getline cleanup");

        fclose(child); // Close the file
        close(pipefd[PIPE_READ]); // Explicitly close pipe

        if(MEM_INFO_D) fprintf(stderr, "[I]: Finished parsing cmus metadata\n");
    }
} /*}}}*/

void create_status(const struct cmus_state * const cs, /*{{{*/
                   struct presence_state * const ps) {
    assert(cs != NULL);
    assert(ps != NULL);

    // State format: {{{
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
    // }}}

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
    } else if(cs->file) {
        // file
        const char * fname_start = strrchr(cs->file, '/');
        fname_start++; // One character after last slash

        state_len += strnlen(fname_start, 512);

        ps->state = (char*) xmallocn(state_len+1, "create state");
        strncpy(ps->state, fname_start, state_len+1);
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
        ps->details = (char*) xmalloc(1);
        ps->details[0] = '\0';
    }

    ps->time_left = cs->duration - cs->position;

    if(MEM_INFO_D) fprintf(stderr, "[I]: Finished creating status from metadata\n");
} /*}}}*/
