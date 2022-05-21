#ifndef __DISCORD_STUFF_H
#define __DISCORD_STUFF_H

#include "../discord-rpc/include/discord_rpc.h"

extern const char * const APP_ID;

struct presence_state {
    char *state;
    char *details;
    int time_left;
};

void free_presence_state(struct presence_state * const ps);

// Functions to notify of events
void discord_on_ready(const DiscordUser * const connected_user);
void discord_on_error(int error_code, const char * const message);
void discord_on_disconnected(int error_code, const char * const message);

// Initialize rpc connection
void discord_init();

// End rpc connection
extern void (*discord_shutdown)(void);

// Add rpc status change to queue
void discord_update_presence(const struct presence_state * const ps);

// Clear rpc status
extern void (*discord_clear_presence)(void);

#endif
