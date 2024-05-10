/*
** server.c -- a stream socket server demo
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>

#define PORT    "3490"
#define BACKLOG 10

void    sigchld_handler(int s);
void    *get_in_addr(struct sockaddr *sa);

int     main(void)
{
   int              sockfd; // return socket function call
   int              new_fd; // socket fd by client
   int              yes; // paremeter to setcockopt call
   int              rv; // return from getaddrinfo call to check 
   char             s[INET6_ADDRSTRLEN];
   struct addrinfo  hints; // Define parameters to getaddrinfo function call
   struct addrinfo  *servinfo; // save addresses from getaddrinfo call
   struct addrinfo  *p; // interete over servinfo
   struct sockaddr_storage their_addr;
   socklen_t        sin_size;
   struct sigaction sa;

   memset(&hints, 0, sizeof(hints));
   hints.ai_family = AF_UNSPEC;
   hints.ai_socktype = SOCK_STREAM;
   hints.ai_flags = AI_PASSIVE; // use server ip

   rv = getaddrinfo(NULL, PORT, &hints, &servinfo);
   if (rv == -1)
   {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return (1);
   }

   // loop througn all the results and bind to the first we can
   for (p = servinfo; p != NULL; p = p->ai_next)
   {
        sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (sockfd == -1)
        {
            perror("server: socket");
            continue;
        }
        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
        {
            perror("setcockopt");
            exit(1);
        }
        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1)
        {
            close(sockfd);
            perror("server: bind");
            continue;
        }
        break;
   }
    freeaddrinfo(servinfo);

    if (p == NULL)
    {
        fprintf(stderr, "server: failed to bind\n");
        exit(1);
    }

    if (listen(sockfd, BACKLOG) == -1)
    {
        perror("listen");
        exit(1);
    }

    sa.sa_handler = sigchld_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1)
    {
        perror("sigaction");
        exit(1);
    }

    printf("server: waiting for connections...\n");

    while(1) 
    {
        sin_size = sizeof(their_addr);
        new_fd = accept(sockfd, (struct sockaddr*)&their_addr, &sin_size);
        if (new_fd == -1)
        {
            perror("accept");
            continue;
        }
        inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr*)&their_addr), s, sizeof(s));
        printf("server: got connection from %s\n", s);
        if (!fork())
        {
            close(sockfd);
            if (send(new_fd, "Hello, world!", 13, 0) == -1)
                perror("send");
            close(new_fd);
            exit(0);
        }
        close(new_fd);
    }
    return (0);
}

void    sigchld_handler(int s) 
{
    int     saved_errno;
    
    saved_errno = errno;    
    while (waitpid(-1, NULL, WNOHANG) > 0);
    errno = saved_errno;
}

void    *get_in_addr(struct sockaddr *sa) 
{
    if (sa->sa_family == AF_INET)
        return (&(((struct sockaddr_in*)sa)->sin_addr));
    return (&(((struct sockaddr_in6*)sa)->sin6_addr));
}
