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
    struct mq_attr attr;
    char buffer[MSG_SIZE];

    // Устанавливаем атрибуты очереди
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = MSG_SIZE;
    attr.mq_curmsgs = 0;

    // Создаем очередь сообщений для сервера
    server_mq = mq_open(SERVER_QUEUE_NAME, O_CREAT | O_RDWR, 0644, &attr);
    if (server_mq == (mqd_t)-1) {
        perror("mq_open (server)");
        return 1;
    }

    // Создаем очередь сообщений для клиента
    client_mq = mq_open(CLIENT_QUEUE_NAME, O_CREAT | O_RDWR, 0644, &attr);
    if (client_mq == (mqd_t)-1) {
        perror("mq_open (client)");
        return 1;
    }

    // Отправляем сообщение клиенту
    strcpy(buffer, "Hi!");
    if (mq_send(client_mq, buffer, strlen(buffer) + 1, 0) == -1) {
        perror("mq_send (client)");
        return 1;
    }
    printf("Sent to client: %s\n", buffer);

    // Ждем ответа от клиента
    if (mq_receive(server_mq, buffer, MSG_SIZE, NULL) == -1) {
        perror("mq_receive (server)");
        return 1;
    }
    printf("Received from client: %s\n", buffer);

    // Закрываем и удаляем очереди сообщений
    if (mq_close(server_mq) == -1) {
        perror("mq_close (server)");
        return 1;
    }
    if (mq_close(client_mq) == -1) {
        perror("mq_close (client)");
        return 1;
    }
    if (mq_unlink(SERVER_QUEUE_NAME) == -1) {
        perror("mq_unlink (server)");
        return 1;
    }
    if (mq_unlink(CLIENT_QUEUE_NAME) == -1) {
        perror("mq_unlink (client)");
        return 1;
    }

    return 0;
}