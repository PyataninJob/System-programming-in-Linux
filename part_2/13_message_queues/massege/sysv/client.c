#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define MSG_SIZE 100

struct message {
    long msg_type;
    char msg_text[MSG_SIZE];
};

int main() {
    key_t key = ftok("progfile", 65); // создаем уникальный ключ
    if (key == -1) {
        perror("ftok");
        return 1;
    }

    int msgid = msgget(key, 0666 | IPC_CREAT); // подключаемся к очереди сообщений
    if (msgid == -1) {
        perror("msgget");
        return 1;
    }

    struct message msg;

    // считываем сообщение от сервера
    if (msgrcv(msgid, &msg, sizeof(msg.msg_text), 1, 0) == -1) {
        perror("msgrcv");
        return 1;
    }
    printf("Received: %s\n", msg.msg_text);

    msg.msg_type = 2;
    strcpy(msg.msg_text, "Hello!");

    if (msgsnd(msgid, &msg, sizeof(msg.msg_text), 0) == -1) { // отправляем ответ серверу
        perror("msgsnd");
        return 1;
    }

    printf("Sent: %s\n", msg.msg_text);

    return 0;
}