#Notas de Estudos do Beej's Guide to Network Programming

Introudução ao estudos de _socket programming_ com a linguagem C em ambientes Linux.

# Estruturas de dados 

## struct addrinfo

Utilizada para representar endereçs de redes ou de conexão. 

```c
struct addrinfo {
    int              ai_flags;     // AI_PASSIVE: modo passivo, escuta; AI_CANONNAME: nome do host canônico.
    int              ai_family;    // AF_INET, AF_INET6, AF_UNSPEC: Se é IPv4 ou IPv6, ou ambos, respectivamente
    int              ai_socktype;  // SOCK_STREAM, SOCK_DGRAM: Indicar o tipo de socket
    int              ai_protocol;  // use 0 for "any": Indicar o tipo de protocolo (IPPROTO_TCP ou IPPROTO_UDP para casos epecíficos)
    size_t           ai_addrlen;   // size of ai_addr in bytes: 
    struct sockaddr *ai_addr;      // struct sockaddr_in or _in6: ponteiro aponta para uma struct sockaddr alocada dinamicamente que contém as informações reais do endereço de rede. 
    char            *ai_canonname; // full canonical hostname

    struct addrinfo *ai_next;      // linked list, next node: Quando getaddrinfo encontra vários endereços possíveis para um hostname, ela cria uma lista vinculada de nós struct addrinfo, 
                                   // onde cada nó representa uma opção de endereço. ai_next
};

```

Em termos de tamanho de memória, as estruturas `struct sockaddr_in` e `struct sockaddr` tem o mesmo tamanho. Permitiando ao programados realizar a operação de _cast_ de uma para outra.

## struct sockaddr

É uma estrutura genérica para armazenar informações a respeito de endereços de socket.

```c 
struct sockaddr {
    unsigned short    sa_family;    // address family, AF_xxx: Este campo indica a família de endereços do socket.
    char              sa_data[14];  // 14 bytes of protocol address: Este array de 14 bytes serve como um contêiner para armazenar a informação específica do endereço do socket, como o 
                                    // endereço IP de destino e o número da porta.
}; 
```

## struct sockaddr_in


```c 
struct sockaddr_in {
    short int          sin_family;  // Address family, AF_INET
    unsigned short int sin_port;    // Port number
    struct in_addr     sin_addr;    // Internet address
    unsigned char      sin_zero[8]; // Same size as struct sockaddr
};
```

## struct in_addr 

```c
struct in_addr {
    uint32_t s_addr; // that's a 32-bit int (4 bytes)
};
```


## struct sockaddr_in6

```c
struct sockaddr_in6 {
    u_int16_t       sin6_family;   // address family, AF_INET6
    u_int16_t       sin6_port;     // port number, Network Byte Order
    u_int32_t       sin6_flowinfo; // IPv6 flow information
    struct in6_addr sin6_addr;     // IPv6 address
    u_int32_t       sin6_scope_id; // Scope ID
};

struct in6_addr {
    unsigned char   s6_addr[16];   // IPv6 address
};
```

# Funções 

## getaddrinfo()

*Man pages descriptions:*
> network address and service translation
> Given  node  and  service,  which  identify an Internet host and a service, getaddrinfo() returns one or more addrinfo structures, each of which contains an Internet address that can be specified in a call to bind(2) or connect(2).  The getaddrinfo() function combines the functionality provided by the gethostbyname(3) and getservbyname(3) functions into a single interface, but unlike the latter functions, getaddrinfo() is reentrant and  allows programs to eliminate IPv4-versus-IPv6 dependencies.


Suas funcionalidade:
* Pesquisa de DNS (para encontrar o IP a partir de um nome de host)
* Pesquisa de nomes de serviço (como "http" para porta 80)
* Preenchimento automático das estruturas struct addrinfo necessárias para a conexão

```c 
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

int getaddrinfo(const char *node,     // e.g. "www.example.com" or IP
                const char *service,  // e.g. "http" or port number
                const struct addrinfo *hints, // Ponteiro para uma struct addrinfo que é preenchida com informações adicionais para filtrar os resultados.
                struct addrinfo **res); // lista vinculada contendo possíveis endereços e informações de conexão com base nos parâmetros fornecidos.

```

Funções auxiliares: 

```c 
void freeaddrinfo(struct addrinfo *res);
const char *gai_strerror(int errcode);
```