#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>


int     main(int argc, char **argv) 
{
    struct addrinfo     hints;
    struct addrinfo     *res;
    struct sockaddr     *ipv4;
    struct addrinfo     tmp;
    struct addrinfo     *p; 

    memset(&hints, 0, sizeof(struct addrinfo *));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    getaddrinfo("www.google.com", NULL, &hints, &res);
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
    tmp = *res;

    freeaddrinfo(res);
    return (0);
}