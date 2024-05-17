#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <signal.h>

#define PORT 8080
#define MAX_WORKERS 4
#define BUFFER_SIZE 1024

void handle_client(int client_socket) {
    char buffer[BUFFER_SIZE];
    int bytes_read;

    // Ler a requisição do cliente
    bytes_read = read(client_socket, buffer, sizeof(buffer) - 1);
    if (bytes_read < 0) {
        perror("read");
        close(client_socket);
        return;
    }

    buffer[bytes_read] = '\0';
    printf("Requisição recebida:\n%s\n", buffer);

    // Enviar resposta HTTP simples
    const char *response =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n"
        "Content-Length: 13\r\n"
        "\r\n"
        "Hello, World!";
    write(client_socket, response, strlen(response));

    // Fechar a conexão com o cliente
    close(client_socket);
}

void worker_process(int server_socket) {
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    int client_socket;

    while (1) {
        // Aceitar uma nova conexão
        client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_len);
        if (client_socket < 0) {
            perror("accept");
            continue;
        }

        handle_client(client_socket);
    }
}

int main() {
    int server_socket;
    struct sockaddr_in server_addr;
    pid_t pid;

    // Ignorar o sinal SIGPIPE para evitar que o programa seja terminado
    // quando tenta escrever em um socket fechado pelo cliente
    signal(SIGPIPE, SIG_IGN);

    // Criar o socket do servidor
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Configurar o endereço do servidor
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Vincular o socket ao endereço do servidor
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    // Escutar por conexões
    if (listen(server_socket, 10) < 0) {
        perror("listen");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    printf("Servidor web iniciado na porta %d\n", PORT);

    // Criar os processos workers
    for (int i = 0; i < MAX_WORKERS; i++) {
        pid = fork();
        if (pid < 0) {
            perror("fork");
            close(server_socket);
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            // Processo filho (worker)
            worker_process(server_socket);
            exit(EXIT_SUCCESS); // Nunca deve alcançar aqui
        }
    }

    // Processo pai (master) espera indefinidamente
    for (;;) {
        pause();
    }

    close(server_socket);
    return 0;
}
