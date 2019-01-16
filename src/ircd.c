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


int check_for_clients(IRCD *ircd)
{
    fd_set rfds;
    struct timeval tv;
    int retval;

    /* Watch stdin (fd 0) to see when it has input. */
    FD_ZERO(&rfds);
    FD_SET(ircd->socket.fd, &rfds);

    /* Wait up to one second. */
    tv.tv_sec = 1;
    tv.tv_usec = 0;

    retval = select(1, &rfds, NULL, NULL, &tv);
    if (retval == -1) {
        fprintf(stderr, "[check_for_clients] select()\n");
    } else if (retval == 0) {
        printf("[check_for_clients] Timeout occured, no new connection.\n");
    } else {
        printf("[check_for_clients] Client is available now.\n");
    }

    if ( net_accept(&ircd->socket, &ircd->users[ircd->numusers].sock) != -1 ) {
        ++ircd->numusers;
        printf("[check_for_clients] At %d users. [%d]\n", ircd->numusers, ircd->users[ircd->numusers].sock.fd);
    }
    return 0;
}

int recv_from_clients(IRCD *ircd)
{
    fd_set rfds;
    struct timeval tv;
    int retval;
    int i;

    /* Watch stdin (fd 0) to see when it has input. */
    FD_ZERO(&rfds);
    for(i = 0; i < ircd->numusers; i++) {
        printf("[recv_from_clients] Adding user %i to rfds\n", i);
        FD_SET(ircd->users[i].sock.fd, &rfds);
    }

    /* Wait up to one second. */
    tv.tv_sec = 1;
    tv.tv_usec = 0;

    retval = select(i+1, &rfds, NULL, NULL, &tv);
    if (retval == -1) {
        fprintf(stderr, "[recv_from_clients] select() error\n");
    } else if (retval == 0) {
        printf("[recv_from_clients] Timeout occured, no new data.\n");	
    } else {
        printf("[recv_from_clients] Data is available now. [%i]\n", retval);
        for(i = 0; i < ircd->numusers; i++) {
            if(FD_ISSET(ircd->users[i].sock.fd, &rfds)) {
                printf("[recv_from_clients] Data from user #%i\n", i);
            }
        }
    }

    return 0;
}

int main()
{
    bool debug = true; // TODO: Make this a command-line flag.
    IRCD ircd;
    memset(&ircd, 0, sizeof(IRCD));
    ircd.numusers = 0;
    ircd.numchans = 0;
    ircd.exiting = false;

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


    if(debug) {
        reusePort(&ircd); // MAY EXIT.
    }

    while(!ircd.exiting) {
        check_for_clients(&ircd);
        recv_from_clients(&ircd);
    }

    return 0;
}
