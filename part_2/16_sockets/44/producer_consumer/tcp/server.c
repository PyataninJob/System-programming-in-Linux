#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 8080
#define BUFFER_SIZE 1024
#define QUEUE_SIZE 3

pthread_t producer_thread, consumer_threads[QUEUE_SIZE];
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

int client_queue[QUEUE_SIZE];
int queue_size = 0;

void *producer(void *arg) {
    int server_socket = *(int *)arg;
    struct sockaddr_in client_addr;
    socklen_t addr_len = sizeof(client_addr);

    while (1) {
        int new_socket = accept(server_socket, (struct sockaddr *)&client_addr, &addr_len);
        if (new_socket == -1) {
            perror("Accept failed");
            continue;
        }

        printf("New client connected: %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        pthread_mutex_lock(&mutex);
        while (queue_size == QUEUE_SIZE) {
            pthread_cond_wait(&cond, &mutex);
        }
        client_queue[queue_size++] = new_socket;
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex);
    }
}

void *consumer(void *arg) {
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
    int server_socket;
    struct sockaddr_in server_addr;

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

    printf("TCP Server with producer-consumer pattern listening on port %d...\n", PORT);

    // Create producer thread
    pthread_create(&producer_thread, NULL, producer, &server_socket);

    // Create consumer threads
    for (int i = 0; i < QUEUE_SIZE; i++) {
        pthread_create(&consumer_threads[i], NULL, consumer, NULL);
    }

    // Join threads
    pthread_join(producer_thread, NULL);
    for (int i = 0; i < QUEUE_SIZE; i++) {
        pthread_join(consumer_threads[i], NULL);
    }

    close(server_socket);
    return 0;
}