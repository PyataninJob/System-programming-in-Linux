#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>

#define SERVER_QUEUE_NAME  "/server_queue"
#define CLIENT_QUEUE_NAME  "/client_queue"
#define MSG_SIZE           100

int main() {
    mqd_t server_mq, client_mq;
    char buffer[MSG_SIZE];

    // Открываем очередь сообщений для сервера
    server_mq = mq_open(SERVER_QUEUE_NAME, O_RDWR);
    if (server_mq == (mqd_t)-1) {
        perror("mq_open (server)");
        return 1;
    }

    // Открываем очередь сообщений для клиента
    client_mq = mq_open(CLIENT_QUEUE_NAME, O_RDWR);
    if (client_mq == (mqd_t)-1) {
        perror("mq_open (client)");
        return 1;
    }

    // Считываем сообщение от сервера
    if (mq_receive(client_mq, buffer, MSG_SIZE, NULL) == -1) {
        perror("mq_receive (client)");
        return 1;
    }
    printf("Received from server: %s\n", buffer);

    // Отправляем ответ серверу
    strcpy(buffer, "Hello!");
    if (mq_send(server_mq, buffer, strlen(buffer) + 1, 0) == -1) {
        perror("mq_send (server)");
        return 1;
    }
    printf("Sent to server: %s\n", buffer);

    // Закрываем очереди
    if (mq_close(server_mq) == -1) {
        perror("mq_close (server)");
        return 1;
    }
    if (mq_close(client_mq) == -1) {
        perror("mq_close (client)");
        return 1;
    }

    return 0;
}