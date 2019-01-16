#ifndef FLUFFY_CONFIG_H
#define FLUFFY_CONFIG_H

/* Individual users */
#define NICK_LENGTH 30                  // Maximum nick length

/* Channels */
#define CHANNEL_NAME_LENGTH 30          // Maximum length of a channel name
#define CHANNEL_KEY_LENGTH 30           // Maximum length of channel key

#define CHANNEL_MAX_USERS 50            // Maximum users per channel

/* Server */
#define SERVER_MAX_CHANNELS 100         // Maximum number of channels at once
#define SERVER_MAX_USERS 50             // Maximum number of users at once
#define SERVER_MAX_LINKS 20             // Maximum number of linked servers at once
#define CLIENT_CONNECT_PORT "6667"      // Port that clients can connect to (non-ssl)
#define SERVER_CONNECT_PORT "6664"      // Port that servers can connect to (non-ssl)

#define CLIENT_CONNECT_PORT_SSL "6697"  // Port that clients can connect to (ssl)
#define SERVER_CONNECT_PORT_SSL "6665"  // Port that servers can connect to (non-ssl)

#endif
