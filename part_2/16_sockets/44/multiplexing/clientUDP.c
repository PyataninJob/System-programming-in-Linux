#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_IP "127.0.0.1"
#define UDP_PORT 9090
#define BUFFER_SIZE 1024

int main() {
    int sock;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];

    // Создаем UDP-сокет
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("UDP socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Настраиваем адрес сервера
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(UDP_PORT);
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

    printf("UDP client started. Sending requests to server...\n");

    // Отправляем запросы и получаем время от сервера
    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        strcpy(buffer, "Time request");

        // Отправляем запрос серверу
        sendto(sock, buffer, strlen(buffer), 0, (struct sockaddr *)&server_addr, sizeof(server_addr));

        // Получаем ответ от сервера
        memset(buffer, 0, BUFFER_SIZE);
        socklen_t server_len = sizeof(server_addr);
        int bytes_received = recvfrom(sock, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&server_addr, &server_len);
        if (bytes_received <= 0) {
            perror("Error receiving data from server");
            break;
        }
        printf("Server Time: %s", buffer);

        sleep(5); // Запрашиваем время каждые 5 секунд
    }

    close(sock);
    return 0;
}