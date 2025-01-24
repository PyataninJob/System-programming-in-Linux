#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h> 
#include <sys/msg.h> // Очереди сообщений

#define MAX_TEXT 512
#define MAX_USERS 10

// Структура для сообщения
struct message {
    long msg_type;
    char msg_text[MAX_TEXT];
};

// Структура для пользователя
struct user {
    char name[MAX_TEXT];
};

// Массив пользователей
struct user users[MAX_USERS];
int user_count = 0;

// Прототипы функций
void broadcast_message(int msgid, struct message *msg);
void add_user(const char *name);
void send_user_list(int msgid);
void print_user_list();

// Функция для рассылки сообщения всем пользователям
void broadcast_message(int msgid, struct message *msg) {
    for (int i = 0; i < user_count; i++) {
        if (msgsnd(msgid, msg, sizeof(msg->msg_text), 0) == -1) { // Отправка сообщения в очередь
            perror("msgsnd");
            exit(1);
        }
    }
}

// Функция для добавления нового пользователя
void add_user(const char *name) {
    if (user_count < MAX_USERS) {
        strcpy(users[user_count].name, name);
        user_count++;
    }
}

// Функция для отправки списка пользователей
void send_user_list(int msgid) {
    struct message msg;
    msg.msg_type = 2;
    strcpy(msg.msg_text, "");
    for (int i = 0; i < user_count; i++) {
        strcat(msg.msg_text, users[i].name);
        if (i < user_count - 1) {
            strcat(msg.msg_text, ","); // Добавление запятой между именами
        }
    }
    broadcast_message(msgid, &msg); // Рассылка сообщения всем пользователям
}

// Функция для вывода списка пользователей на сервере
void print_user_list() {
    printf("Current users:\n"); 
    for (int i = 0; i < user_count; i++) {
        printf("%s\n", users[i].name);
    }
}

int main() {
    key_t key;
    int msgid;
    struct message msg;

    // Генерация уникального ключа
    key = ftok("server", 65);

    // Создание очереди сообщений
    msgid = msgget(key, 0666 | IPC_CREAT);
    if (msgid == -1) {
        perror("msgget");
        exit(1);
    }

    printf("Server started. Waiting for messages...\n");

    while (1) {
        // Получение сообщения от клиента
        if (msgrcv(msgid, &msg, sizeof(msg.msg_text), 0, 0) == -1) {
            perror("msgrcv");
            exit(1); // Завершение программы с ошибкой
        }

        // Обработка сообщения в зависимости от типа
        if (msg.msg_type == 1) { // Обычное сообщение
            printf("Received message: %s\n", msg.msg_text);
            broadcast_message(msgid, &msg);
        } else if (msg.msg_type == 2) {
            printf("Adding new user: %s\n", msg.msg_text);
            
            add_user(msg.msg_text);
            send_user_list(msgid);
            print_user_list();
        }
    }

    return 0;\
}
