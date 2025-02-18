#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <ctype.h>

#define PORT 8080
#define BUFFER_SIZE 1024

void modify_string(char *str) {
    // Пример модификации строки: заменить все пробелы на символы подчеркивания
    for (int i = 0; str[i]; i++) {
        if (str[i] == ' ') {
            str[i] = '_';
        }
    }
}

int main() {
    int sockfd;
    char buffer[BUFFER_SIZE];
    struct sockaddr_in servaddr, cliaddr;

    // Создание UDP сокета
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Ошибка создания сокета");
        exit(EXIT_FAILURE);
    }
    printf("Сокет создан.\n");

    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));

    // Заполнение информации о сервере
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);

    // Привязка сокета к адресу и порту
    if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("Ошибка привязки");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    printf("Сокет привязан к порту %d.\n", PORT);

    int len, n;
    len = sizeof(cliaddr);

    while (1) {
        // Получение данных от клиента
        n = recvfrom(sockfd, (char *)buffer, BUFFER_SIZE, MSG_WAITALL, (struct sockaddr *)&cliaddr, &len);
        buffer[n] = '\0';
        printf("Получено сообщение от клиента: %s\n", buffer);

        // Модификация строки
        modify_string(buffer);
        printf("Модифицированное сообщение: %s\n", buffer);

        // Отправка модифицированной строки обратно клиенту
        if (sendto(sockfd, (const char *)buffer, strlen(buffer), MSG_CONFIRM, (const struct sockaddr *)&cliaddr, len) < 0) {
            perror("Ошибка отправки");
        } else {
            printf("Модифицированное сообщение отправлено клиенту.\n");
        }
    }

    close(sockfd);
    printf("Сокет закрыт.\n");
    return 0;
}