#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char *message = "Привет";
    char buffer[BUFFER_SIZE] = {0};

    // Создание сокета
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Создание сокета не удалось \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Укажите публичный IP-адрес сервера
    if (inet_pton(AF_INET, "192.168.0.105", &serv_addr.sin_addr) <= 0) {
        printf("\nНеверный адрес/ Адрес не поддерживается \n");
        return -1;
    }

    // Подключение к серверу
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nОшибка подключения \n");
        return -1;
    }

    // Отправка сообщения серверу
    send(sock, message, strlen(message), 0);
    printf("Сообщение отправлено\n");

    // Чтение ответа от сервера
    read(sock, buffer, BUFFER_SIZE);
    printf("Ответ от сервера: %s\n", buffer);

    close(sock);
    return 0;
}