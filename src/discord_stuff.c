#include "discord_stuff.h"

#include "colors.h"
#include "utils.h"
#include <stdio.h>
#include <string.h>

const char * const APP_ID = "976582643938377749";

void free_presence_state(struct presence_state * const ps) { /*{{{*/
    if(MEM_INFO_D) fprintf(stderr, "[I]: Freeing presence_state\n");

    if(ps->state)   nfreen(ps->state,   "presence_state state");
    if(ps->details) nfreen(ps->details, "presence_state details");

    if(MEM_INFO_D) fprintf(stderr, "[I]: Finished freeing presence_state\n");
} /*}}}*/

// Functions to notify of events {{{
void discord_on_ready(const DiscordUser * const connected_user) {
    fprintf(stderr, "[I]: Discord ready!\n");
}

void discord_on_error(int error_code, const char * const message) {
    fprintf(stderr, "%s[E]:%s Discord error!\n", _C(ERROR), _C(CLEAR));
}

void discord_on_disconnected(int error_code, const char * const message) {
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
void discord_update_presence(const struct presence_state * const ps) { /*{{{*/
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
