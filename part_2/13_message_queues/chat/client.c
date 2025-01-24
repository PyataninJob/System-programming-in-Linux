
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/ipc.h> 
#include <sys/msg.h> // Очереди сообщений
#include <pthread.h> // Потоки
#include <ncurses.h> // Интерфейс

#define MAX_TEXT 512
#define MAX_USERS 10

// Сообщения
struct message {
    long msg_type;
    char msg_text[MAX_TEXT];
};

// Окна для интерфейса
WINDOW *messages_win, *users_win, *input_win; // Окна для сообщений, списка пользователей и ввода
int msgid; // Идентификатор очереди сообщений
char users[MAX_USERS][MAX_TEXT]; // Массив имен пользователей
int user_count = 0; // Количество пользователей
char username[MAX_TEXT]; // Имя текущего пользователя

void *receive_messages(void *arg);
void init_windows();
void update_users();
void update_messages(const char *message);

// Функция для получения сообщений в отдельном потоке
void *receive_messages(void *arg) {
    struct message msg; 
    while (1) { 
        // Получение сообщения из очереди
        if (msgrcv(msgid, &msg, sizeof(msg.msg_text), 0, 0) == -1) {
            perror("msgrcv");
            exit(1);
        }
        // Обработка сообщения
        if (msg.msg_type == 1) { 
            update_messages(msg.msg_text); // Обновление окна сообщений
        } else if (msg.msg_type == 2) { 
            char *token = strtok(msg.msg_text, ","); // Разделение строки на имена пользователей
            user_count = 0; // Сброс счетчика пользователей
            while (token != NULL && user_count < MAX_USERS) {
                strcpy(users[user_count++], token);
                token = strtok(NULL, ",");
            }
            update_users(); // Обновление окна списка пользователей

            if (user_count > 0) {
                char join_msg[MAX_TEXT];
                snprintf(join_msg, MAX_TEXT, "**User %s has joined the chat**", users[user_count - 1]);
                update_messages(join_msg); // Обновление окна сообщений
            }
        }
    }
    return NULL;
}

// Функция для инициализации окон интерфейса
void init_windows() {
    int height, width;
    getmaxyx(stdscr, height, width); // Получение размеров стандартного окна

    messages_win = newwin(height - 3, width - 20, 0, 0); 
    users_win = newwin(height - 3, 20, 0, width - 20); 
    input_win = newwin(3, width, height - 3, 0);

    // Отрисовка рамок для окон
    box(messages_win, 0, 0);
    box(users_win, 0, 0);
    box(input_win, 0, 0);

    // Отображение заголовка для окна пользователей
    mvwprintw(users_win, 1, 1, "Users:");
    wrefresh(messages_win);
    wrefresh(users_win);
    wrefresh(input_win);
}

// Функция для обновления списка пользователей
void update_users() {
    werase(users_win); // Очистка окна списка пользователей
    box(users_win, 0, 0); // Отрисовка рамки для окна списка пользователей
    mvwprintw(users_win, 1, 1, "Users:");
    // Вывод списка пользователей
    for (int i = 0; i < user_count; i++) {
        mvwprintw(users_win, i + 2, 1, "%s", users[i]);
    }
    wrefresh(users_win); // Обновление окна списка пользователей
}

void update_messages(const char *message) {
    int max_y, max_x;
    getmaxyx(messages_win, max_y, max_x); // Получение размеров окна
    int n = 2; // Количество строк, занимаемых рамкой

    // Проверка, нужно ли прокручивать окно
    if (getcury(messages_win) >= max_y - n - 1) {
        // Копирование всей области сообщений и перерисовка со смещением на одну строку вверх
        for (int i = 1; i < max_y - n - 1; i++) {
            char buffer[max_x];
            mvwinstr(messages_win, i + 1, 1, buffer); // Копирование строки
            mvwprintw(messages_win, i, 1, "%-*s", max_x - 2, buffer); // Перерисовка строки на одну строку вверх
        }
        // Очистка последней строки
        mvwprintw(messages_win, max_y - n - 1, 1, "%-*s", max_x - 2, " ");
    }

    // Перемещение курсора в конец окна
    wmove(messages_win, max_y - n - 1, 1);

    // Вывод нового сообщения в окно сообщений
    wprintw(messages_win, "%s", message);
    box(messages_win, 0, 0);
    wrefresh(messages_win);
}

int main() {
    key_t key;
    struct message msg;
    pthread_t recv_thread;

    // Генерация уникального ключа
    key = ftok("server", 65);

    // Подключение к очереди сообщений
    msgid = msgget(key, 0666 | IPC_CREAT);
    if (msgid == -1) {
        perror("msgget");
        exit(1);
    }

    // Инициализация ncurses
    initscr(); // Инициализация библиотеки ncurses
    cbreak(); // Отключение буферизации ввода
    echo(); // Включение отображения вводимых символов
    keypad(stdscr, TRUE); // Включение обработки функциональных клавиш

    // Инициализация окон интерфейса
    init_windows();

    // Запуск потока для получения сообщений
    pthread_create(&recv_thread, NULL, receive_messages, NULL);

    // Ввод имени пользователя
    mvwprintw(input_win, 1, 1, "Enter your name: ");
    wrefresh(input_win);
    wgetnstr(input_win, username, MAX_TEXT);

    // Отправка сообщения о новом пользователе серверу
    msg.msg_type = 2;
    snprintf(msg.msg_text, MAX_TEXT, "%s", username);
    if (msgsnd(msgid, &msg, sizeof(msg.msg_text), 0) == -1) {
        perror("msgsnd");
        exit(1);
    }

    msg.msg_type = 1;

    while (1) {
        // Очистка окна ввода
        werase(input_win);
        box(input_win, 0, 0); // Отрисовка рамки для окна ввода сообщений
        mvwprintw(input_win, 1, 1, "Enter message: "); // Вывод приглашения для ввода сообщения
        wrefresh(input_win); // Обновление окна ввода сообщений

        // Получение строки ввода от пользователя
        wgetnstr(input_win, msg.msg_text, MAX_TEXT);

        // Добавление имени пользователя к сообщению
        char full_msg[MAX_TEXT];
        snprintf(full_msg, MAX_TEXT, "%s: %s", username, msg.msg_text);
        strncpy(msg.msg_text, full_msg, MAX_TEXT);

        // Отправка сообщения серверу
        if (msgsnd(msgid, &msg, sizeof(msg.msg_text), 0) == -1) {
            perror("msgsnd");
            exit(1);
        }
    }

    
    endwin(); // Завершение работы с ncurses
    return 0;
}
