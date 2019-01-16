#ifndef FLUFFY_NETWORK_H
#define FLUFFY_NETWORK_H

#include "config.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

typedef struct SockInfo_s {
    struct addrinfo *res;
    struct addrinfo hints;
    int fd;
    int backlog;
    struct sockaddr_storage ip;
    socklen_t ip_size;
} SockInfo;

bool net_socket(SockInfo *sock, char *port, int backlog);
int net_bind(SockInfo *sock);
int net_listen(SockInfo *sock);
int net_accept(SockInfo *sock, SockInfo *usersock);

#endif
