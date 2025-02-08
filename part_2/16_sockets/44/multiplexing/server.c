#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define TCP_PORT 8080
#define UDP_PORT 9090
#define BUFFER_SIZE 1024

void *handle_tcp_client(void *arg) {
    int client_socket = *(int *)arg;
    free(arg);
    char buffer[BUFFER_SIZE];
    time_t current_time;

    while (1) {
        time(&current_time);
        snprintf(buffer, BUFFER_SIZE, "TCP Server Time: %s", ctime(&current_time));
        send(client_socket, buffer, strlen(buffer), 0);
        sleep(5); // Отправляем время каждые 5 секунд
    }

    close(client_socket);
    return NULL;
}

void *handle_udp_client(void *arg) {
    int udp_socket = *(int *)arg;
    char buffer[BUFFER_SIZE];
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    time_t current_time;

    while (1) {
        recvfrom(udp_socket, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&client_addr, &client_len);
        time(&current_time);
        snprintf(buffer, BUFFER_SIZE, "UDP Server Time: %s", ctime(&current_time));
        sendto(udp_socket, buffer, strlen(buffer), 0, (struct sockaddr *)&client_addr, client_len);
    }

    return NULL;
}

int main() {
    int tcp_socket, udp_socket, *client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    pthread_t udp_thread;

    // TCP socket
    tcp_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (tcp_socket < 0) {
        perror("TCP socket creation failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(TCP_PORT);

    if (bind(tcp_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("TCP bind failed");
        close(tcp_socket);
        exit(EXIT_FAILURE);
    }

    listen(tcp_socket, 5);

    // UDP socket
    udp_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (udp_socket < 0) {
        perror("UDP socket creation failed");
        close(tcp_socket);
        exit(EXIT_FAILURE);
    }

    server_addr.sin_port = htons(UDP_PORT);

    if (bind(udp_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("UDP bind failed");
        close(udp_socket);
        close(tcp_socket);
        exit(EXIT_FAILURE);
    }

    printf("Server is running...\n");

    // Create a thread to handle UDP clients
    pthread_create(&udp_thread, NULL, handle_udp_client, &udp_socket);

    while (1) {
        // Handle TCP connections
        client_socket = malloc(sizeof(int));
        *client_socket = accept(tcp_socket, (struct sockaddr *)&client_addr, &client_len);
        if (*client_socket < 0) {
            perror("TCP accept failed");
            free(client_socket);
            continue;
        }

        pthread_t tcp_thread;
        pthread_create(&tcp_thread, NULL, handle_tcp_client, client_socket);
        pthread_detach(tcp_thread); // Отсоединяем поток, чтобы он мог завершиться самостоятельно
    }

    close(tcp_socket);
    close(udp_socket);
    return 0;
}