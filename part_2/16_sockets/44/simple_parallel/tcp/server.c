#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 8080
#define BUFFER_SIZE 1024

void *handle_client(void *arg) {
    int client_socket = *(int *)arg;
    char buffer[BUFFER_SIZE];
    int bytes_read;

    while ((bytes_read = read(client_socket, buffer, BUFFER_SIZE)) > 0) {
        buffer[bytes_read] = '\0';
        printf("Received: %s\n", buffer);
        write(client_socket, buffer, bytes_read); // Echo back
    }

    close(client_socket);
    free(arg);
    pthread_exit(NULL);
}

int main() {
    int server_socket, new_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);
    pthread_t tid;

    // Create socket
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Bind
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Bind failed");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    // Listen
    if (listen(server_socket, 5) == -1) {
        perror("Listen failed");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    printf("TCP Server listening on port %d...\n", PORT);

    while (1) {
        // Accept new connection
        if ((new_socket = accept(server_socket, (struct sockaddr *)&client_addr, &addr_len)) == -1) {
            perror("Accept failed");
            continue;
        }

        printf("New client connected: %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        // Create a new thread for each client
        int *client_socket = malloc(sizeof(int));
        *client_socket = new_socket;
        if (pthread_create(&tid, NULL, handle_client, client_socket) != 0) {
            perror("Thread creation failed");
            free(client_socket);
            close(new_socket);
        }
    }

    close(server_socket);
    return 0;
}