#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int sockfd;
    struct sockaddr_in servaddr;
    char *message = "Привет";
    char buffer[BUFFER_SIZE];

    // Создание сокета
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Сокет не создан");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);

    // Укажите публичный IP-адрес сервера
    if (inet_pton(AF_INET, "192.168.0.105", &servaddr.sin_addr) <= 0) {
        printf("\nНеверный адрес/ Адрес не поддерживается \n");
        return -1;
    }

    // Отправка сообщения серверу
    sendto(sockfd, (const char *)message, strlen(message), MSG_CONFIRM, (const struct sockaddr *)&servaddr, sizeof(servaddr));
    printf("Сообщение отправлено\n");

    int len, n;
    len = sizeof(servaddr);

    // Чтение ответа от сервера
    n = recvfrom(sockfd, (char *)buffer, BUFFER_SIZE, MSG_WAITALL, (struct sockaddr *)&servaddr, &len);
    buffer[n] = '\0';
    printf("Ответ от сервера: %s\n", buffer);

    close(sockfd);
    return 0;
}