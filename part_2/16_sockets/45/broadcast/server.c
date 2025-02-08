#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BROADCAST_PORT 9090
#define BROADCAST_IP "255.255.255.255"
#define BUFFER_SIZE 1024

int main() {
    int sock;
    struct sockaddr_in broadcast_addr;
    char buffer[BUFFER_SIZE];
    int broadcast_enable = 1;

    // Создание сокета
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Разрешение отправки широковещательных сообщений
    if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &broadcast_enable, sizeof(broadcast_enable)) < 0) {
        perror("Error in setting Broadcast option");
        close(sock);
        exit(EXIT_FAILURE);
    }

    // Настройка адреса для отправки
    memset(&broadcast_addr, 0, sizeof(broadcast_addr));
    broadcast_addr.sin_family = AF_INET;
    broadcast_addr.sin_port = htons(BROADCAST_PORT);
    inet_pton(AF_INET, BROADCAST_IP, &broadcast_addr.sin_addr);

    while (1) {
        snprintf(buffer, BUFFER_SIZE, "Broadcast message at %ld", time(NULL));
        if (sendto(sock, buffer, strlen(buffer), 0, (struct sockaddr *)&broadcast_addr, sizeof(broadcast_addr)) < 0) {
            perror("Broadcast send failed");
        }
        printf("Broadcast message sent: %s\n", buffer);
        sleep(5); // Отправка сообщения каждые 5 секунд
    }

    close(sock);
    return 0;
}