#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8080
#define BUFFER_SIZE 1024
#define HEADER_SIZE 8

typedef struct custom_header {
    uint16_t src_port;
    uint16_t dst_port;
    uint16_t length;
    uint16_t checksum;
} custom_header;

int main() {
    int sockfd;
    struct sockaddr_in server_addr, sender_addr;
    socklen_t addr_len = sizeof(sender_addr);

    // Создание сокета
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Настройка адреса сервера
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);

    // Заполнение заголовка
    custom_header header = {
        .src_port = htons(12345),  // Придуманный порт клиента
        .dst_port = htons(PORT),   // Порт сервера
        .length = htons(sizeof(custom_header) + strlen("Hello, Server!")),
        .checksum = 0              // Чексумма не используется
    };

    char buffer[BUFFER_SIZE];
    memset(buffer, 0, BUFFER_SIZE);  // Очистка буфера

    // Копирование заголовка и данных в буфер
    memcpy(buffer, &header, sizeof(custom_header));
    strncpy(buffer + sizeof(custom_header), "Hello, Server!", BUFFER_SIZE - sizeof(custom_header));

    // Отправка данных на сервер
    if (sendto(sockfd, buffer, ntohs(header.length), 0,
               (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Sendto failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    printf("Packet sent successfully.\n");

    // Получение ответа от сервера
    ssize_t bytes_received = recvfrom(sockfd, buffer, BUFFER_SIZE, 0,
                                       (struct sockaddr *)&sender_addr, &addr_len);
    if (bytes_received < 0) {
        perror("Recvfrom failed");
    } else if (bytes_received > HEADER_SIZE) {
        buffer[bytes_received] = '\0';
        printf("Received response from server: %s\n", buffer+HEADER_SIZE);

        // Проверка src_port для определения, является ли это ответом от сервера
        custom_header *recv_header = (custom_header *)buffer;
        if (recv_header->dst_port == htons(PORT)) {
            printf("Received response from server: %s\n", buffer + HEADER_SIZE);
        } else {
            printf("Ignored packet from unknown source.\n");
        }
    } else {
        printf("Received packet too small to contain data.\n");
    }

    close(sockfd);
    return 0;
}