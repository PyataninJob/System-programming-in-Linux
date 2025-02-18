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
    struct sockaddr_un servaddr, cliaddr;
    char buffer[BUFFER_SIZE];
    char *response = "Здарова";

    // Создание сокета
    if ((sockfd = socket(AF_LOCAL, SOCK_DGRAM, 0)) < 0) {
        perror("Сокет не создан");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));

    servaddr.sun_family = AF_LOCAL;
    strncpy(servaddr.sun_path, SOCKET_PATH, sizeof(servaddr.sun_path) - 1);

    // Привязка сокета к пути
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

    sleep(3);

    // Отправка ответа клиенту
    sendto(sockfd, (const char *)response, strlen(response), MSG_CONFIRM, (const struct sockaddr *)&cliaddr, len);
    printf("Ответ отправлен\n");

    close(sockfd);
    unlink(SOCKET_PATH); // Удаляем сокет
    return 0;
}