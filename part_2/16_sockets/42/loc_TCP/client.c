#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SOCKET_PATH "/home/pyatanin/VSproj/C/part_2/16_sockets/42/loc_TCP/my_tcp_socket"
#define BUFFER_SIZE 1024

int main() {
    int sock = 0;
    struct sockaddr_un serv_addr;
    char *message = "Привет";
    char buffer[BUFFER_SIZE] = {0};

    // Создание сокета
    if ((sock = socket(AF_LOCAL, SOCK_STREAM, 0)) < 0) {
        printf("\n Создание сокета не удалось \n");
        return -1;
    }

    serv_addr.sun_family = AF_LOCAL;
    strncpy(serv_addr.sun_path, SOCKET_PATH, sizeof(serv_addr.sun_path) - 1);

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