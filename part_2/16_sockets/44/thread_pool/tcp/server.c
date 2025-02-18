#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 8080
#define BUFFER_SIZE 1024
#define POOL_SIZE 5

pthread_t pool[POOL_SIZE];
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

int client_queue[POOL_SIZE];
int queue_size = 0;

void *handle_client(void *arg) {
    while (1) {
        pthread_mutex_lock(&mutex);
        while (queue_size == 0) {
            pthread_cond_wait(&cond, &mutex);
        }
        int client_socket = client_queue[--queue_size];
        pthread_mutex_unlock(&mutex);

        char buffer[BUFFER_SIZE];
        int bytes_read;

        while ((bytes_read = read(client_socket, buffer, BUFFER_SIZE)) > 0) {
            buffer[bytes_read] = '\0';
            printf("Received: %s\n", buffer);
            write(client_socket, buffer, bytes_read); // Echo back
        }

        close(client_socket);
    }
}

int main() {
    int server_socket, new_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);

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

    printf("TCP Server with thread pool listening on port %d...\n", PORT);

    // Initialize thread pool
    for (int i = 0; i < POOL_SIZE; i++) {
        pthread_create(&pool[i], NULL, handle_client, NULL);
    }

    while (1) {
        // Accept new connection
        if ((new_socket = accept(server_socket, (struct sockaddr *)&client_addr, &addr_len)) == -1) {
            perror("Accept failed");
            continue;
        }

        printf("New client connected: %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        // Add client to queue
        pthread_mutex_lock(&mutex);
        client_queue[queue_size++] = new_socket;
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex);
    }

    close(server_socket);
    return 0;
}