#include <sys/select.h>
#include <unistd.h>
#include "network.h"
#include "ircd.h"


// Allow the port to be reused.
// Helpful for debugging.
// See: http://beej.us/guide/bgnet/output/html/singlepage/bgnet.html#bind
void reusePort(IRCD *ircd)
{
    int yes=1;
    //char yes='1'; // Solaris people use this

    if (setsockopt(ircd->socket.fd,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int)) == -1) {
        perror("setsockopt");
        exit(1);
    }
}


ssize_t recv_from_user(IRCD *ircd, int usernum)
{
    User *user = &ircd->users[usernum];
    int fd = user->sock.fd;
    char buffer[513];
    ssize_t ret = recv(fd, &buffer, 512, 0);

    if (ret < 0) {
        printf("[read_from_user] Error during recv().\n");
        return ret;
    }

    // TODO: Handle anything beyond the expected 512 bytes.

    printf("[%i %s] RECV %zi: '%s'\n", usernum, "", ret, buffer);

    return ret;
}

int accept_client(IRCD *ircd)
{
    int ret = net_accept(&ircd->socket, &ircd->users[ircd->numusers].sock);

    if (ret < 0) {
        printf("[accept_client] Error with net_accept().\n");
        return ret;
    }

    ircd->numusers++;
    printf("[check_for_clients] At %d users. [%d]\n", ircd->numusers, ircd->users[ircd->numusers].sock.fd);

    return 0;
}

int poll_sockets(IRCD *ircd)
{
    fd_set rfds;
    struct timeval tv;
    int retval;
    int i;

    /* Watch stdin (fd 0) to see when it has input. */
    FD_ZERO(&rfds);
    FD_SET(ircd->socket.fd, &rfds);
    for(i = 0; i < ircd->numusers; i++) {
        if (ircd->debug) {
            printf("[poll_sockets] Adding user %i to rfds.\n", i);
        }
        FD_SET(ircd->users[i].sock.fd, &rfds);
    }

    /* Wait up to one second. */
    tv.tv_sec = 1;
    tv.tv_usec = 0;

//    retval = select(i + 2, &rfds, NULL, NULL, &tv);
    // TODO: Actually set nfds (first arg) correctly.
    retval = select(1000, &rfds, NULL, NULL, &tv);

    if (retval == -1) {
        // TODO: Include error information from errno.
        fprintf(stderr, "[poll_sockets] select() error\n");
        return -1;
    }

    if (retval == 0) {
        if (ircd->debug) {
            printf("[poll_sockets] Timeout occured, no new data.\n");
        }
        return 0;
    }

    printf("[poll_sockets] Data is available now. [%i]\n", retval);

    if (FD_ISSET(ircd->socket.fd, &rfds)) {
        printf("[poll_sockets] New connection.\n");
        accept_client(ircd);
    }

    for(i = 0; i < ircd->numusers; i++) {
        if(FD_ISSET(ircd->users[i].sock.fd, &rfds)) {
            if (ircd->debug) {
                printf("[poll_sockets] Data from user #%i\n", i);
            }
            recv_from_user(ircd, i);
        }
    }

    return 0;
}

int main()
{
    IRCD ircd;
    memset(&ircd, 0, sizeof(IRCD));
    ircd.numusers = 0;
    ircd.numchans = 0;
    ircd.exiting = false;
    ircd.debug = true; // TODO: Make this a command-line flag.

    if (!net_socket(&(ircd.socket), CLIENT_CONNECT_PORT, 0)) {
        printf("[main] Error creating socket.\r\n");
        return 1;
    }
    if (net_bind(&(ircd.socket)) < 0) {
        printf("[main] Error binding socket.\r\n");
        return 1;
    }
    if (net_listen(&(ircd.socket)) < 0) {
        printf("[main] Error listening on socket.\r\n");
        return 1;
    }

    /*
       strcpy(ircd->users[0].nick, "test");

       ircd->channels[0].users[0] = ircd->users[0];

       printf("Nick of first user in channel: %s; user's nick: %s\n", ircd->channels[0].users[0].nick, ircd->users[0].nick);
       */

    if(ircd.debug) {
        reusePort(&ircd); // MAY EXIT.
    }

    while(!ircd.exiting) {
        poll_sockets(&ircd);
    }

    return 0;
}
