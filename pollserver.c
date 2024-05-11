/*
** pollserver.c -- a cheezy multiperson chat server
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <poll.h>

#define     PORT "9034"

void        *get_in_addr(struct sockaddr *sa) 
{
    if (sa->sa_family == AF_INET)
        return (&(((struct sockaddr_in*)sa)->sin_addr));
    return (&(((struct sockaddr_in6*)sa)->sin6_addr));
}

int         get_listener_socket(void)
{
    int             listener;
    int             yes;
    int             rv;
    struct addrinfo hints, *ai, *p;

    yes = 1;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    rv = getaddrinfo(NULL, PORT, &hints, &ai);
    if (rv != 0)
    {
        fprintf(stderr, "server: %s\n", gai_strerror(rv))
        exit(1);
    }
    for (p = ai; p != NULL; p->ai_next)
    {
        listener = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (listener < 0)
            continue;
        setsockopt(listener, SOL_SOCKET, SOL_REUSEADDR, &yes, sizeof(int));

        if (bind(listener, p->ai_addr, p->ai_addrlen) < 0)
        {
            close(listener);
            continue;
        }
        break;
    }

    freeaddrinfo(ai);

    if (p == NULL) return (-1);
    if (listen(listener, 10) == -1) return (-1);
    return (listener);
}

void        add_to_pfds(struct pollfd *pfds[], int newfd, int *fd_count, int *fd_size)
{
    if (*fd_count == *fd_size)
    {
        *fd_size *= 2;
        *pfds = realloc(*pfds, sizeof(**pfds) * (*fd_size));
    }

    (*pfds)[*fd_count].fd = newfd;
    (*pfds)[*fd_count].events = POLLIN;

    (*fd_count)++;
}

void        del_from_pfds(struct pollfd *pfds[], int i, int *fd_count)
{
    pfds[i] = pfds[*fd_count - 1];
    (*fd_count)--;
}

int         main(void)
{
    int         listener;
    int         newfd;
    int         fd_count;
    int         fd_size;
    char        buf[256];
    char        remoteIP[INET6_ADDRSTRLEN];
    socklen_t   addrlen;
    struct pollfd *pfds;
    struct sockaddr_storage remoteaddr;

    fd_count = 0;
    fd_size = 5;
    pfds = malloc(sizeof(*pfds) * fd_size);
    listener = get_listener_socket();
    if (listener == -1)
    {
        fprintf(stderr, "error getting listening socket\n");
        exit(1);
    }

    pfds[0].fd = listener;
    pfds[0].events = POLLIN;

    fd_count = 1;

    while(1)
    {
        int poll_count = poll(pfds, fd_count, -1);
        if (poll_count == -1)
        {
            perror("poll");
            exit(1);
        }
    }

    return (0);
}