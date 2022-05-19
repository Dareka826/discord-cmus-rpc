#include "discord-rpc/include/discord_rpc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

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

int main() {
    discord_init();

    discord_update_presence();
    sleep(10);

    discord_shutdown();
    return EXIT_SUCCESS;
}
