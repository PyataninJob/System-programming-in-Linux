#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};
    char *response = "Здарова";

    // Создание сокета
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Сокет не создан");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY; // Принимаем соединения на все интерфейсы
    address.sin_port = htons(PORT);

    // Привязка сокета к порту
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Привязка не удалась");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Ожидание подключения клиента
    if (listen(server_fd, 3) < 0) {
        perror("Ошибка listen");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Ожидание подключения клиента...\n");
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        perror("Ошибка accept");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Чтение сообщения от клиента
    read(new_socket, buffer, BUFFER_SIZE);
    printf("Сообщение от клиента: %s\n", buffer);

    // Отправка ответа клиенту
    send(new_socket, response, strlen(response), 0);
    printf("Ответ отправлен\n");

    close(new_socket);
    close(server_fd);
    return 0;
}