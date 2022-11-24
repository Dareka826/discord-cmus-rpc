#include "colors.h"
#include "utils.h"
#include "discord_stuff.h"
#include "cmus_stuff.h"
#include "snippets/arena/arena.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

#ifndef _VERSION
#define _VERSION "<no version>"
#endif

volatile sig_atomic_t requested_exit = 0;

void handle_exit(int signum) {
    requested_exit = 1;
}

int main() {
    printf("[I]: cmus-rpcd version: %s\n", _VERSION);
    discord_init();

    signal(SIGTERM, handle_exit);
    signal(SIGINT,  handle_exit);

    Arena *cs_a = NULL,
          *ps_a = NULL,
          *old_ps_a = create_arenan("old_ps");

    struct cmus_state cs;
    struct presence_state ps, old_ps;

    int old_status = 0;

    memset(&old_ps, 0, sizeof(old_ps));
    old_ps.state   = (char*) arena_xalloc0(old_ps_a, 1);
    old_ps.details = (char*) arena_xalloc0(old_ps_a, 1);

    // Main logic {{{
    while(requested_exit == 0) {
        _mem_info_log("\n"); // Better visibility
        cs_a = create_arenan("cs_a");
        ps_a = create_arenan("ps_a");

        // Clear out variables
        memset(&cs, 0, sizeof(cs));
        memset(&ps, 0, sizeof(ps));

        cmus_get_metadata(cs_a, &cs);

        // cmus is playing
        if(cs.status == 1) {
            create_status(ps_a, &cs, &ps);

            int changed = 0;
            // Check if status changed from last time {{{
            {
                int old_state_len   = strnlen(old_ps.state,   512);
                int old_details_len = strnlen(old_ps.details, 512);
                int new_state_len   = strnlen(    ps.state,   512);
                int new_details_len = strnlen(    ps.details, 512);

                if( (old_status == 0) || // Started playing
                    (old_state_len   != new_state_len)   || // Length doesn't match
                    (old_details_len != new_details_len) || // Length doesn't match
                    (strncmp(old_ps.state,   ps.state,   MAX(old_state_len,   new_state_len))   != 0) ||  // Strings don't match
                    (strncmp(old_ps.details, ps.details, MAX(old_details_len, new_details_len)) != 0) ) { // Strings don't match

                    changed = 1;
                }
            } /*}}}*/

            if(changed != 0) {
                printf("[I]: Status changed!\n");

                printf("State: %s\n", ps.state);
                printf("Deets: %s\n", ps.details);
                //printf("Timel: %d\n", ps.time_left);

                discord_update_presence(&ps);
            }

            old_status = 1;

            // Free old data
            nfree_arena(old_ps_a);

            // Move new data into old
            _mem_info_log("%s[MEM]%s ps_a -> old_ps_a\n", _C(GREEN), _C(CLEAR));
            old_ps_a = ps_a;
            ps_a = NULL;

            memcpy(&old_ps, &ps, sizeof(struct presence_state));

        } else {
            // Not playing
            if(old_status == 1) {
                printf("[I]: Stopped playing\n");
                old_status = 0;

                discord_clear_presence();
            }
        }

        nfree_arena(cs_a);

        _mem_info_log("\n"); // Better visibility

        // Prevent messages from piling-up
        fflush(stderr);
        fflush(stdout);

        sleep(1);
    } /*}}}*/

    discord_shutdown();
    return EXIT_SUCCESS;
}
