#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SOCKET_PATH "/home/pyatanin/VSproj/C/part_2/16_sockets/42/loc_UDP/my_udp_socket"
#define BUFFER_SIZE 1024

int main() {
    int sockfd;
    struct sockaddr_un servaddr;
    char *message = "Привет";
    char buffer[BUFFER_SIZE];

    // Создание сокета
    if ((sockfd = socket(AF_LOCAL, SOCK_DGRAM, 0)) < 0) {
        perror("Сокет не создан");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));

    servaddr.sun_family = AF_LOCAL;
    strncpy(servaddr.sun_path, SOCKET_PATH, sizeof(servaddr.sun_path) - 1);

    // Отправка сообщения серверу
    sendto(sockfd, (const char *)message, strlen(message), MSG_CONFIRM, (const struct sockaddr *)&servaddr, sizeof(servaddr));
    printf("Сообщение отправлено\n");
    
    // Ожидание ответа от сервера
    int len = sizeof(servaddr);
    int n = recvfrom(sockfd, (char *)buffer, BUFFER_SIZE, MSG_WAITALL, (struct sockaddr *)&servaddr, &len);
    if (n < 0) {
        perror("Ошибка при получении ответа");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    buffer[n] = '\0';
    printf("Ответ от сервера: %s\n", buffer);
    close(sockfd);
    return 0;
}