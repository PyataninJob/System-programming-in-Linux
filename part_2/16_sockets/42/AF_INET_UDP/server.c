#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int sockfd;
    struct sockaddr_in servaddr, cliaddr;
    char buffer[BUFFER_SIZE];
    char *response = "Здарова";

    // Создание сокета
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Сокет не создан");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY; // Принимаем соединения на все интерфейсы
    servaddr.sin_port = htons(PORT);

    // Привязка сокета к порту
    if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("Привязка не удалась");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("Ожидание сообщения от клиента...\n");
    int len, n;
    len = sizeof(cliaddr);

    // Чтение сообщения от клиента
    n = recvfrom(sockfd, (char *)buffer, BUFFER_SIZE, MSG_WAITALL, (struct sockaddr *)&cliaddr, &len);
    buffer[n] = '\0';
    printf("Сообщение от клиента: %s\n", buffer);

    // Отправка ответа клиенту
    sendto(sockfd, (const char *)response, strlen(response), MSG_CONFIRM, (const struct sockaddr *)&cliaddr, len);
    printf("Ответ отправлен\n");

    close(sockfd);
    return 0;
}