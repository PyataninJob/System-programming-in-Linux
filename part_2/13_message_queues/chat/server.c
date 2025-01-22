#include <stdio.h> // Подключение стандартной библиотеки ввода-вывода
#include <stdlib.h> // Подключение стандартной библиотеки для работы с памятью и процессами
#include <string.h> // Подключение библиотеки для работы со строками
#include <sys/ipc.h> // Подключение библиотеки для работы с IPC (межпроцессное взаимодействие)
#include <sys/msg.h> // Подключение библиотеки для работы с очередями сообщений

#define MAX_TEXT 512 // Определение максимальной длины текста сообщения
#define MAX_USERS 10 // Определение максимального количества пользователей

// Структура для сообщения
struct message {
    long msg_type; // Тип сообщения
    char msg_text[MAX_TEXT]; // Текст сообщения
};

// Структура для пользователя
struct user {
    char name[MAX_TEXT]; // Имя пользователя
};

// Массив пользователей
struct user users[MAX_USERS]; // Массив структур пользователей
int user_count = 0; // Количество пользователей

// Прототипы функций
void broadcast_message(int msgid, struct message *msg);
void add_user(const char *name);
void send_user_list(int msgid);
void print_user_list();

// Функция для рассылки сообщения всем пользователям
void broadcast_message(int msgid, struct message *msg) {
    for (int i = 0; i < user_count; i++) { // Проход по всем пользователям
        if (msgsnd(msgid, msg, sizeof(msg->msg_text), 0) == -1) { // Отправка сообщения в очередь
            perror("msgsnd"); // Вывод ошибки, если отправка не удалась
            exit(1); // Завершение программы с ошибкой
        }
    }
}

// Функция для добавления нового пользователя
void add_user(const char *name) {
    if (user_count < MAX_USERS) { // Проверка, что количество пользователей не превышает максимум
        strcpy(users[user_count].name, name); // Копирование имени пользователя в массив
        user_count++; // Увеличение счетчика пользователей
    }
}

// Функция для отправки списка пользователей
void send_user_list(int msgid) {
    struct message msg; // Создание структуры сообщения
    msg.msg_type = 2; // Тип сообщения для списка пользователей
    strcpy(msg.msg_text, ""); // Инициализация текста сообщения
    for (int i = 0; i < user_count; i++) { // Проход по всем пользователям
        strcat(msg.msg_text, users[i].name); // Добавление имени пользователя в сообщение
        if (i < user_count - 1) { // Если это не последний пользователь
            strcat(msg.msg_text, ","); // Добавление запятой между именами
        }
    }
    broadcast_message(msgid, &msg); // Рассылка сообщения всем пользователям
}

// Функция для вывода списка пользователей на сервере
void print_user_list() {
    printf("Current users:\n"); // Вывод заголовка
    for (int i = 0; i < user_count; i++) { // Проход по всем пользователям
        printf("%s\n", users[i].name); // Вывод имени пользователя
    }
}

int main() {
    key_t key; // Переменная для хранения ключа
    int msgid; // Переменная для хранения идентификатора очереди сообщений
    struct message msg; // Переменная для хранения сообщения

    // Генерация уникального ключа
    key = ftok("server", 65); // Генерация ключа на основе файла и идентификатора

    // Создание очереди сообщений
    msgid = msgget(key, 0666 | IPC_CREAT); // Создание очереди сообщений с правами доступа 0666
    if (msgid == -1) { // Проверка на ошибку создания очереди
        perror("msgget"); // Вывод ошибки
        exit(1); // Завершение программы с ошибкой
    }

    printf("Server started. Waiting for messages...\n"); // Вывод сообщения о старте сервера

    while (1) { // Бесконечный цикл для обработки сообщений
        // Получение сообщения от клиента
        if (msgrcv(msgid, &msg, sizeof(msg.msg_text), 0, 0) == -1) { // Получение сообщения любого типа
            perror("msgrcv"); // Вывод ошибки
            exit(1); // Завершение программы с ошибкой
        }

        // Обработка сообщения в зависимости от типа
        if (msg.msg_type == 1) { // Обычное сообщение
            printf("Received message: %s\n", msg.msg_text); // Вывод полученного сообщения
            broadcast_message(msgid, &msg); // Рассылка сообщения всем пользователям
        } else if (msg.msg_type == 2) { // Сообщение о новом пользователе
            printf("Adding new user: %s\n", msg.msg_text); // Вывод имени нового пользователя
            
            add_user(msg.msg_text); // Добавление нового пользователя
            send_user_list(msgid); // Отправка обновленного списка пользователей
            print_user_list(); // Вывод списка пользователей на сервере
        }
    }

    return 0; // Завершение программы
}
