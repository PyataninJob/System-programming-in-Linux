#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SOCKET_PATH "/home/pyatanin/VSproj/C/part_2/16_sockets/42/loc_TCP/my_tcp_socket"
#define BUFFER_SIZE 1024

int main() {
    int server_fd, new_socket;
    struct sockaddr_un address;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};
    char *response = "Здарова";

    // Создание сокета
    if ((server_fd = socket(AF_LOCAL, SOCK_STREAM, 0)) == 0) {
        perror("Сокет не создан");
        exit(EXIT_FAILURE);
    }

    address.sun_family = AF_LOCAL;
    strncpy(address.sun_path, SOCKET_PATH, sizeof(address.sun_path) - 1);

    // Привязка сокета к пути
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
    unlink(SOCKET_PATH); // Удаляем сокет
    return 0;
}