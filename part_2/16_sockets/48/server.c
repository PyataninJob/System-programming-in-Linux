#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

void modify_message(char *message) {
    char modified[BUFFER_SIZE];
    snprintf(modified, sizeof(modified), "Modified: %s", message);
    strncpy(message, modified, BUFFER_SIZE - 1);
}

int main() {
    int sockfd;
    char buffer[BUFFER_SIZE];
    struct sockaddr_in servaddr, cliaddr;

    // Создание UDP сокета
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));

    // Заполнение информации о сервере
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);

    // Привязка сокета к адресу и порту
    if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("Bind failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    printf("Server is running on port %d...\n", PORT);

    int len;
    len = sizeof(cliaddr);

    while (1) {
        // Получение данных от клиента
        ssize_t bytes_received = recvfrom(sockfd, (char *)buffer, BUFFER_SIZE, MSG_WAITALL, (struct sockaddr *)&cliaddr, &len);
        if (bytes_received > 0) {
            buffer[bytes_received] = '\0';
            printf("Received message from client: %s\n", buffer);

            // Модификация сообщения
            modify_message(buffer);
            printf("Modified message: %s\n", buffer);

            // Отправка модифицированной строки обратно клиенту
            if (sendto(sockfd, (const char *)buffer, strlen(buffer), MSG_CONFIRM, (const struct sockaddr *)&cliaddr, len) < 0) {
                perror("Sendto failed");
            } else {
                printf("Sent modified message back to client.\n");
            }
        }
    }

    close(sockfd);
    printf("Сокет закрыт.\n");
    return 0;
}