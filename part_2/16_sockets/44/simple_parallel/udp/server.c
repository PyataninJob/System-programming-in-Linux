#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 8080
#define BUFFER_SIZE 1024

void *handle_client(void *arg) {
    int server_socket = *(int *)arg;
    struct sockaddr_in client_addr;
    socklen_t addr_len = sizeof(client_addr);
    char buffer[BUFFER_SIZE];
    int bytes_read;

    while (1) {
        bytes_read = recvfrom(server_socket, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&client_addr, &addr_len);
        if (bytes_read <= 0) {
            perror("Receive failed");
            continue;
        }

        buffer[bytes_read] = '\0';
        printf("Received from %s:%d: %s\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port), buffer);

        // Echo back
        sendto(server_socket, buffer, bytes_read, 0, (struct sockaddr *)&client_addr, addr_len);
    }
}

int main() {
    int server_socket;
    struct sockaddr_in server_addr;
    pthread_t tid;

    // Create socket
    if ((server_socket = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
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

    printf("UDP Server listening on port %d...\n", PORT);

    // Create a thread to handle clients
    if (pthread_create(&tid, NULL, handle_client, &server_socket) != 0) {
        perror("Thread creation failed");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    pthread_join(tid, NULL); // Wait for the thread
    close(server_socket);
    return 0;
}