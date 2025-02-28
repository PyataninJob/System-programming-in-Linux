#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/select.h>

#define TCP_PORT 8080
#define UDP_PORT 9090
#define BUFFER_SIZE 1024
#define MAX_CLIENTS 10

int main() {
    int tcp_socket, udp_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    fd_set read_fds, active_fds;
    int max_fd;
    char buffer[BUFFER_SIZE];
    time_t current_time;

    // Массив для хранения активных TCP-соединений
    int client_sockets[MAX_CLIENTS];
    for (int i = 0; i < MAX_CLIENTS; i++) {
        client_sockets[i] = -1; // Инициализируем как неиспользуемые
    }

    // TCP socket
    tcp_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (tcp_socket < 0) {
        perror("TCP socket creation failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(TCP_PORT);

    if (bind(tcp_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("TCP bind failed");
        close(tcp_socket);
        exit(EXIT_FAILURE);
    }

    listen(tcp_socket, 5);

    // UDP socket
    udp_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (udp_socket < 0) {
        perror("UDP socket creation failed");
        close(tcp_socket);
        exit(EXIT_FAILURE);
    }

    server_addr.sin_port = htons(UDP_PORT);

    if (bind(udp_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("UDP bind failed");
        close(udp_socket);
        close(tcp_socket);
        exit(EXIT_FAILURE);
    }

    printf("Server is running...\n");

    // Инициализируем набор файловых дескрипторов
    FD_ZERO(&active_fds);
    FD_SET(tcp_socket, &active_fds);
    FD_SET(udp_socket, &active_fds);
    max_fd = (tcp_socket > udp_socket) ? tcp_socket : udp_socket;

    while (1) {
        read_fds = active_fds;

        // Вызываем select для мониторинга активности
        if (select(max_fd + 1, &read_fds, NULL, NULL, NULL) < 0) {
            perror("select failed");
            continue;
        }

        // Проверяем, есть ли новое TCP-соединение
        if (FD_ISSET(tcp_socket, &read_fds)) {
            int client_socket = accept(tcp_socket, (struct sockaddr *)&client_addr, &client_len);
            if (client_socket < 0) {
                perror("TCP accept failed");
                continue;
            }

            // Добавляем новое соединение в массив клиентов
            for (int i = 0; i < MAX_CLIENTS; i++) {
                if (client_sockets[i] == -1) {
                    client_sockets[i] = client_socket;
                    FD_SET(client_socket, &active_fds); // Добавляем в набор дескрипторов
                    if (client_socket > max_fd) {
                        max_fd = client_socket; 
                    }
                    printf("New TCP client connected. Socket: %d\n", client_socket);
                    break;
                }
            }
        }

        // Проверяем, есть ли данные от UDP-клиента
        if (FD_ISSET(udp_socket, &read_fds)) {
            recvfrom(udp_socket, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&client_addr, &client_len);
            time(&current_time);
            snprintf(buffer, BUFFER_SIZE, "UDP Server Time: %s", ctime(&current_time));
            sendto(udp_socket, buffer, strlen(buffer), 0, (struct sockaddr *)&client_addr, client_len);
        }

        // Отправляем время всем активным TCP-клиентам
        time(&current_time);
        snprintf(buffer, BUFFER_SIZE, "TCP Server Time: %s", ctime(&current_time));

        for (int i = 0; i < MAX_CLIENTS; i++) {
            if (client_sockets[i] != -1) {
                if (send(client_sockets[i], buffer, strlen(buffer), 0) < 0) {
                    // Если отправка не удалась, закрываем сокет и удаляем его из набора
                    perror("Send failed");
                    close(client_sockets[i]);
                    FD_CLR(client_sockets[i], &active_fds);
                    client_sockets[i] = -1;
                }
            }
        }

        sleep(5); // Отправляем время каждые 5 секунд
    }

    // Закрываем все сокеты 
    close(tcp_socket);
    close(udp_socket);
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (client_sockets[i] != -1) {
            close(client_sockets[i]);
        }
    }

    return 0;
}