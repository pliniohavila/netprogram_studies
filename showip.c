/*
** showip.c -- show IP addresses for a host given on the command line
*/
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>


int     main(int argc, char **argv) 
{
    struct addrinfo     hints; // Define hostname parameters to search
    struct addrinfo     *res;  // Linked List where will be save the ips resolved by hostname
    struct addrinfo     *p;    // Temporaly object that will be serve to interate over res
    int                 status; // Return by call getaddrinfo()
    char                ipstr[INET6_ADDRSTRLEN]; // IP representing in string format
    void                *addr;
    char                *ipver; // Save ip version in string format
    struct sockaddr_in  *ipv4;
    struct sockaddr_in6 *ipv6;

    if (argc != 2)
    {
        fprintf(stderr, "usage: %s hostname\n", argv[0]);
        return (1);
    }
    memset(&hints, 0, sizeof(struct addrinfo *));
    hints.ai_family = AF_UNSPEC; // AF_INET or AF_INET6
    hints.ai_socktype = SOCK_STREAM; // We will handle only TCP sockets
    status = getaddrinfo(
        argv[1], // hostname to address lookup
        NULL, // service or protocol
        &hints, // Additional information for filtering results
        &res // List of all addresses resolved by hostname
    );
    if (status != 0)
    {
        fprintf(stderr, "getaddrinfo erro: %s\n", gai_strerror(status));
        return (2);
    }
    printf("IP addresses for %s:\n", argv[1]);
    for (p = res; p != NULL; p = p->ai_next)
    {
        if (p->ai_family == AF_INET) // if IPv4
        {
            ipv4 = (struct sockaddr_in *)p->ai_addr;
            addr = &(ipv4->sin_addr);
            ipver = "IPv4";
        }
        else
        {
            ipv6 = (struct sockaddr_in6*)p->ai_addr;
            addr = &(ipv6->sin6_addr);
            ipver = "IPv6";
        }
        inet_ntop(p->ai_family, addr, ipstr, sizeof(ipstr));
        printf("%s:%s\n", ipver, ipstr);
    }
    freeaddrinfo(res);
    return (0);
}