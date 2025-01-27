#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <pthread.h>
#include <ncurses.h>
#include <unistd.h>
#include "shared_memory.h"

// Окна для интерфейса
WINDOW *messages_win, *users_win, *input_win;
char username[MAX_TEXT]; // Имя текущего пользователя
struct shared_memory *shm; // Указатель на разделяемую память

void *receive_messages(void *arg);
void init_windows();
void update_users();
void update_messages();

int main() {
    key_t key; // Ключ для сегмента разделяемой памяти
    int shmid; // Идентификатор сегмента разделяемой памяти
    pthread_t recv_thread; // Поток для получения сообщений

    // Генерация уникального ключа
    key = ftok("server", 65);

    // Подключение к сегменту разделяемой памяти
    shmid = shmget(key, sizeof(struct shared_memory), 0666);
    if (shmid == -1) {
        perror("shmget");
        exit(1);
    }

    shm = (struct shared_memory *)shmat(shmid, NULL, 0);
    if (shm == (void *)-1) {
        perror("shmat");
        exit(1);
    }

    // Инициализация ncurses
    initscr();
    cbreak();
    echo();
    keypad(stdscr, TRUE);

    // Инициализация окон интерфейса
    init_windows();

    // Запуск потока для получения сообщений
    pthread_create(&recv_thread, NULL, receive_messages, NULL);

    // Ввод имени пользователя
    mvwprintw(input_win, 1, 1, "Enter your name: ");
    wrefresh(input_win);
    wgetnstr(input_win, username, MAX_TEXT);

    // Добавление пользователя в разделяемую память
    pthread_mutex_lock(&shm->mutex);
    if (shm->user_count < MAX_USERS) {
        strcpy(shm->users[shm->user_count], username);
        shm->user_count++;
    }
    pthread_mutex_unlock(&shm->mutex);

    // Отправка системного сообщения о подключении нового пользователя
    char connect_msg[MAX_TEXT];
    snprintf(connect_msg, MAX_TEXT, "System: User %.500s has joined the chat.\n", username);
    pthread_mutex_lock(&shm->mutex);
    if (shm->message_count < MAX_MESSAGES) {
        strcpy(shm->messages[shm->message_count], connect_msg);
        shm->message_count++;
    } else {
        for (int i = 1; i < MAX_MESSAGES; i++) {
            strcpy(shm->messages[i - 1], shm->messages[i]);
        }
        strcpy(shm->messages[MAX_MESSAGES - 1], connect_msg);
    }
    pthread_mutex_unlock(&shm->mutex);

    while (1) {
        // Очистка окна ввода
        werase(input_win);
        box(input_win, 0, 0);
        mvwprintw(input_win, 1, 1, "Enter message: ");
        wrefresh(input_win);

        // Получение строки ввода от пользователя
        char input[MAX_TEXT];
        wgetnstr(input_win, input, MAX_TEXT);

        // Добавление имени пользователя к сообщению
        char full_msg[MAX_TEXT];
        snprintf(full_msg, MAX_TEXT, "%.250s: %.250s\n", username, input);

        // Отправка сообщения в разделяемую память
        pthread_mutex_lock(&shm->mutex);
        if (shm->message_count < MAX_MESSAGES) {
            strcpy(shm->messages[shm->message_count], full_msg);
            shm->message_count++;
        } else {
            // Если буфер сообщений заполнен, сдвигаем все сообщения на одну позицию вверх
            for (int i = 1; i < MAX_MESSAGES; i++) {
                strcpy(shm->messages[i - 1], shm->messages[i]);
            }
            strcpy(shm->messages[MAX_MESSAGES - 1], full_msg);
        }
        pthread_mutex_unlock(&shm->mutex);
    }

    endwin();
    return 0;
}

// Функция для получения сообщений в отдельном потоке
void *receive_messages(void *arg) {
    int last_message_count = 0; // Последнее количество сообщений
    while (1) {
        pthread_mutex_lock(&shm->mutex);
        if (shm->message_count != last_message_count) { // Проверка, изменилось ли количество сообщений
            last_message_count = shm->message_count; // Обновление последнего количества сообщений
            update_messages(); // Обновление окна сообщений
            update_users(); // Обновление окна пользователей
        }
        pthread_mutex_unlock(&shm->mutex);
        sleep(1); // Ожидание перед следующей проверкой
    }
    return NULL;
}

// Функция для инициализации окон интерфейса
void init_windows() {
    int height, width;
    getmaxyx(stdscr, height, width); // Получение размеров стандартного окна

    messages_win = newwin(height - 3, width - 20, 0, 0); // Создание окна для сообщений
    users_win = newwin(height - 3, 20, 0, width - 20); // Создание окна для списка пользователей
    input_win = newwin(3, width, height - 3, 0); // Создание окна для ввода сообщений

    box(messages_win, 0, 0); // Отрисовка рамки для окна сообщений
    box(users_win, 0, 0); // Отрисовка рамки для окна списка пользователей
    box(input_win, 0, 0); // Отрисовка рамки для окна ввода сообщений

    mvwprintw(users_win, 1, 1, "Users:"); // Отображение заголовка для окна пользователей
    wrefresh(messages_win);
    wrefresh(users_win);
    wrefresh(input_win);
}

// Функция для обновления списка пользователей
void update_users() {
    werase(users_win); // Очистка окна списка пользователей
    box(users_win, 0, 0); // Отрисовка рамки для окна списка пользователей
    mvwprintw(users_win, 1, 1, "Users:"); // Отображение заголовка для окна пользователей
    for (int i = 0; i < shm->user_count; i++) { // Проход по всем пользователям
        mvwprintw(users_win, i + 2, 1, "%s", shm->users[i]); // Вывод имени пользователя
    }
    wrefresh(users_win); // Обновление окна списка пользователей
}

// Функция для обновления окна сообщений
void update_messages() {
    int max_y, max_x;
    getmaxyx(messages_win, max_y, max_x); // Получение размеров окна сообщений

    werase(messages_win); // Очистка окна сообщений

    // Вывод последних 10 сообщений
    int start = shm->message_count > max_y-2 ? shm->message_count - (max_y-2) : 0;
    for (int i = start; i < shm->message_count; i++) {
        mvwprintw(messages_win, i - start + 1, 1, "%s", shm->messages[i]);
    }
    box(messages_win, 0, 0); // Отрисовка рамки для окна сообщений

    wrefresh(messages_win); // Обновление окна сообщений
}