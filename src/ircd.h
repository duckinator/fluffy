#ifndef FLUFFY_IRCD_H
#define FLUFFY_IRCD_H

#include <stdint.h>
#include <stdbool.h>
#include "network.h"
#include "config.h"

/*typedef struct {
    int ip[4];
    int type;
} IP;*/

typedef struct {
    char nick[NICK_LENGTH];
    SockInfo sock;
    int *next;
} User;

typedef struct {
    char name[CHANNEL_NAME_LENGTH];
    char key[CHANNEL_KEY_LENGTH];
    int number_of_users;
    User users[CHANNEL_MAX_USERS];
    int *next;
} Channel;

typedef struct {
    SockInfo socket;
    User users[SERVER_MAX_USERS];
    Channel channels[SERVER_MAX_CHANNELS];
    int numusers;
    int numchans;
    bool exiting;
} IRCD;

#endif
