#include "colors.h"
#include "utils.h"
#include "discord_stuff.h"
#include "cmus_stuff.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main() {
    // TODO:
    // - change status on pause/play
    // - change song if metadata changes
    // - free stuff on SIGTERM

    // - if status != "playing", stop displaying rpc info
    // - if song data suddenly changes (check every 1 sec), reset rpc data

    discord_init();

    struct cmus_state cs;
    struct presence_state ps, old_ps;

    int old_status = 0;
    memset(&old_ps, 0, sizeof(old_ps));
    old_ps.state = (char*) malloc(1);
    old_ps.state[0] = '\0';
    old_ps.details = (char*) malloc(1);
    old_ps.details[0] = '\0';

    // Main logic {{{
    while(1) {
        if(MEM_INFO_D) fprintf(stderr, "\n");

        // Clear out variables
        memset(&cs, 0, sizeof(cs));
        memset(&ps, 0, sizeof(ps));

        cmus_get_metadata(&cs);

        if(cs.status == 1) {
            create_status(&cs, &ps);

            int changed = 0;
            {
                int old_state_len = strnlen(old_ps.state, 512);
                int new_state_len = strnlen(    ps.state, 512);
                int old_details_len = strnlen(old_ps.details, 512);
                int new_details_len = strnlen(    ps.details, 512);

                if( (old_status == 0) ||
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

            old_status = 1;

            free_presence_state(&old_ps); // Free old data
            memcpy(&old_ps, &ps, sizeof(struct presence_state));

        } else {
            // Not playing
            if(old_status == 1) {
                printf("[I]: Stopped playing\n");
                old_status = 0;

                discord_clear_presence();
            }
        }

        free_cmus_state(&cs);
        if(MEM_INFO_D) fprintf(stderr, "\n");

        // Prevent messages from piling-up
        fflush(stderr);
        fflush(stdout);

        sleep(1);
    } /*}}}*/

    discord_shutdown();
    return EXIT_SUCCESS;
}
