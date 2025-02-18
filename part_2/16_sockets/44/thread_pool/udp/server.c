#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 8080
#define BUFFER_SIZE 1024
#define POOL_SIZE 3

pthread_t pool[POOL_SIZE];
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

struct client_request {
    struct sockaddr_in client_addr;
    char buffer[BUFFER_SIZE];
    int bytes_read;
};

struct client_request client_queue[POOL_SIZE];
int queue_size = 0;

void *handle_client(void *arg) {
    while (1) {
        pthread_mutex_lock(&mutex);
        while (queue_size == 0) {
            pthread_cond_wait(&cond, &mutex);
        }
        struct client_request req = client_queue[--queue_size];
        pthread_mutex_unlock(&mutex);

        // Echo back
        sendto(*(int *)arg, req.buffer, req.bytes_read, 0, (struct sockaddr *)&req.client_addr, sizeof(req.client_addr));
    }
}

int main() {
    int server_socket;
    struct sockaddr_in server_addr;

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

    printf("UDP Server with thread pool listening on port %d...\n", PORT);

    // Initialize thread pool
    for (int i = 0; i < POOL_SIZE; i++) {
        pthread_create(&pool[i], NULL, handle_client, &server_socket);
    }

    while (1) {
        struct sockaddr_in client_addr;
        socklen_t addr_len = sizeof(client_addr);
        char buffer[BUFFER_SIZE];

        int bytes_read = recvfrom(server_socket, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&client_addr, &addr_len);
        if (bytes_read <= 0) {
            perror("Receive failed");
            continue;
        }

        buffer[bytes_read] = '\0';
        printf("Received from %s:%d: %s\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port), buffer);

        // Add request to queue
        pthread_mutex_lock(&mutex);
        client_queue[queue_size].client_addr = client_addr;
        memcpy(client_queue[queue_size].buffer, buffer, bytes_read);
        client_queue[queue_size].bytes_read = bytes_read;
        queue_size++;
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex);
    }

    close(server_socket);
    return 0;
}