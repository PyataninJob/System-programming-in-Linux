#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <fcntl.h>
#include <unistd.h>

#define MSG_SIZE 100

struct message {
    long msg_type;
    char msg_text[MSG_SIZE];
};

int main() {
    key_t key = ftok("server", 65); // создаем уникальный ключ
    if (key == -1) {
        perror("ftok");
        return 1;
    }

    int msgid = msgget(key, 0666 | IPC_CREAT); // создаем очередь сообщений
    if (msgid == -1) {
        perror("msgget");
        return 1;
    }

    struct message msg;
    msg.msg_type = 1;
    strcpy(msg.msg_text, "Hi!");

    if (msgsnd(msgid, &msg, sizeof(msg.msg_text), 0) == -1) { // отправляем сообщение
        perror("msgsnd");
        return 1;
    }

    printf("Sent: %s\n", msg.msg_text);

    // ждем ответа от клиента
    if (msgrcv(msgid, &msg, sizeof(msg.msg_text), 2, 0) == -1) {
        perror("msgrcv");
        return 1;
    }
    printf("Received: %s\n", msg.msg_text);

    // удаляем очередь сообщений
    if (msgctl(msgid, IPC_RMID, NULL) == -1) {
        perror("msgctl");
        return 1;
    }

    return 0;
}