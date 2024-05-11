/*
** talker.c -- a datagram "client" demo
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define SERVERPORT "4950"
#define HOSTNAME   "localhost" 

int     main(int argc, char **argv)
{
    int                 sockfd;
    int                 rv;
    int                 numbytes;
    struct addrinfo     hints;
    struct addrinfo     *servinfo;
    struct addrinfo     *p;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET6;
    hints.ai_socktype = SOCK_DGRAM;

    rv = getaddrinfo(HOSTNAME, SERVERPORT, &hints, &servinfo);
    if (rv != 0)
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return (1);
    }
    for (p = servinfo; p != NULL; p = p->ai_next)
    {
        if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
        {
            perror("talker: socket");
            continue;
        }
        break;
    }
    if (p == NULL)
    {
        fprintf(stderr, "talker: failed to create socket\n");
        return (2);
    }
    numbytes = sendto(sockfd, argv[1], strlen(argv[1]), 0, p->ai_addr, p->ai_addrlen);
    if (numbytes == -1)
    {
        perror("talker: sendto");
        exit(1);
    }

    freeaddrinfo(servinfo);

    printf("talker: send %d bytes to %s\n", numbytes, HOSTNAME);
    close(sockfd);
    return (0);
}