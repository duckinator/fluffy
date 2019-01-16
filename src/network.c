#include "network.h"

SockInfo get_socket(char *port, int backlog)
{
    SockInfo sock;

    if (0 == backlog) {
        backlog = 10;
    }
    sock.backlog = backlog;

    // TODO: Error checking?

    memset(&sock.hints, 0, sizeof sock.hints);
    sock.hints->ai_family = AF_UNSPEC;       // Use either IPv4 or IPv6.
    sock.hints->ai_socktype = SOCK_STREAM;   // ???
    sock.hints->ai_flags = AI_PASSIVE;       // Automagically determine my own IP.
    getaddrinfo(NULL, port, sock.hints, &sock.res);

    // Make a socket, bind it, and listen on it.
    sock.fd = socket(sock.res->ai_family, sock.res->ai_socktype, sock.res->ai_protocol);

    return sock;
}

int net_bind(SockInfo sock)
{
    return bind(sock.fd, sock.res->ai_addr, sock.res->ai_addrlen);
}

int net_listen(SockInfo sock)
{
    return listen(sock.fd, sock.backlog);
}

int net_accept(SockInfo sock, SockInfo usersock)
{
    usersock.ip_size = sizeof usersock.ip;
    usersock.fd = accept(sock.fd, (struct sockaddr *)&usersock.ip, &usersock.ip_size);
    printf("[net_accept] Accepted user. sockfd: %i; ip: %s; ipsize: %i\n", (int)sock.fd, "", (int)usersock.ip_size);

    if (usersock.fd > -1) {
        // If it's a proper file descriptor, we already stored it, so
        // just return a generic success indicator.
        return 0;
    } else {
        // If it's an error, pass it on through.
        return usersock.fd;
    }
}
