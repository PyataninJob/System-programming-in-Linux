
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define MULTICAST_PORT 9090
#define MULTICAST_IP "239.0.0.1"
#define BUFFER_SIZE 1024

int main() {
    int sock;
    struct sockaddr_in multicast_addr;
    char buffer[BUFFER_SIZE];

    // Создание сокета
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Настройка адреса для отправки
    memset(&multicast_addr, 0, sizeof(multicast_addr));
    multicast_addr.sin_family = AF_INET;
    multicast_addr.sin_port = htons(MULTICAST_PORT);
    inet_pton(AF_INET, MULTICAST_IP, &multicast_addr.sin_addr);

    while (1) {
        snprintf(buffer, BUFFER_SIZE, "Multicast message at %ld", time(NULL));
        if (sendto(sock, buffer, strlen(buffer), 0, (struct sockaddr *)&multicast_addr, sizeof(multicast_addr)) < 0) {
            perror("Multicast send failed");
        }
        printf("Multicast message sent: %s\n", buffer);
        sleep(5); // Отправка сообщения каждые 5 секунд
    }

    close(sock);
    return 0;
}