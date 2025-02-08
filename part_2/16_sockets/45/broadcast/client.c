#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BROADCAST_PORT 9090
#define BUFFER_SIZE 1024

int main() {
    int sock;
    struct sockaddr_in recv_addr;
    char buffer[BUFFER_SIZE];
    socklen_t addr_len = sizeof(recv_addr);
    int reuse = 1;

    // Создание сокета
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Разрешение повторного использования адреса
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0) {
        perror("Setting SO_REUSEADDR failed");
        close(sock);
        exit(EXIT_FAILURE);
    }

    // Настройка адреса для приема
    memset(&recv_addr, 0, sizeof(recv_addr));
    recv_addr.sin_family = AF_INET;
    recv_addr.sin_addr.s_addr = INADDR_ANY;
    recv_addr.sin_port = htons(BROADCAST_PORT);

    // Привязка сокета к адресу
    if (bind(sock, (struct sockaddr *)&recv_addr, sizeof(recv_addr)) < 0) {
        perror("Bind failed");
        close(sock);
        exit(EXIT_FAILURE);
    }

    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        if (recvfrom(sock, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&recv_addr, &addr_len) < 0) {
            perror("Receive failed");
        }
        printf("Received broadcast message: %s\n", buffer);
    }

    close(sock);
    return 0;
}