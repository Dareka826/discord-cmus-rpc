#ifndef __DISCORD_STUFF_H
#define __DISCORD_STUFF_H

#include "../discord-rpc/include/discord_rpc.h"

const char * const APP_ID = "976582643938377749";

struct presence_state {
    char *state;
    char *details;
    int time_left;
};

void free_presence_state(struct presence_state *ps);

// Functions to notify of events
void discord_on_ready(const DiscordUser *connected_user);
void discord_on_error(int error_code, const char *message);
void discord_on_disconnected(int error_code, const char *message);

// Initialize rpc connection
void discord_init();

// End rpc connection
extern void (*discord_shutdown)(void);

// Add rpc status change to queue
void discord_update_presence(struct presence_state *ps);

// Clear rpc status
extern void (*discord_clear_presence)(void);

#endif
