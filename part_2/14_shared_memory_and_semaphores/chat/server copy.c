#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include "shared_memory.h"

// Функция для рассылки сообщения всем пользователям
void broadcast_message(struct shared_memory *shm, const char *message) {
    pthread_mutex_lock(&shm->mutex); // Захват мьютекса
    if (shm->message_count < MAX_MESSAGES) {
        strcpy(shm->messages[shm->message_count], message); // Добавление сообщения в буфер
        shm->message_count++; // Увеличение счетчика сообщений
    } else {
        // Если буфер сообщений заполнен, сдвигаем все сообщения на одну позицию вверх
        for (int i = 1; i < MAX_MESSAGES; i++) {
            strcpy(shm->messages[i - 1], shm->messages[i]);
        }
        strcpy(shm->messages[MAX_MESSAGES - 1], message);
    }
    pthread_mutex_unlock(&shm->mutex); // Освобождение мьютекса
    printf("Broadcast message: %s\n", message); // Вывод сообщения на сервере
}

// Функция для отправки списка пользователей всем клиентам
void broadcast_user_list(struct shared_memory *shm) {
    char user_list[MAX_TEXT] = "Users: ";
    for (int i = 0; i < shm->user_count; i++) {
        strcat(user_list, shm->users[i]);
        if (i < shm->user_count - 1) {
            strcat(user_list, ", ");
        }
    }
    broadcast_message(shm, user_list);
}

// Функция для добавления нового пользователя
void add_user(struct shared_memory *shm, const char *name) {
    pthread_mutex_lock(&shm->mutex); // Захват мьютекса
    if (shm->user_count < MAX_USERS) { // Проверка, что количество пользователей не превышает максимум
        strcpy(shm->users[shm->user_count], name); // Копирование имени пользователя в массив
        shm->user_count++; // Увеличение счетчика пользователей
        printf("User %s has connected.\n", name); // Сообщение о подключении пользователя

        // Отправка сообщения о подключении нового пользователя
        char connect_msg[MAX_TEXT];
        snprintf(connect_msg, MAX_TEXT, "System: User %s has joined the chat.\n", name);
        broadcast_message(shm, connect_msg);

        // Отправка обновленного списка пользователей
        broadcast_user_list(shm);
    }
    pthread_mutex_unlock(&shm->mutex); // Освобождение мьютекса
}

// Функция для обработки сообщений от клиентов
void handle_messages(struct shared_memory *shm) {
    char last_message[MAX_TEXT] = ""; // Последнее полученное сообщение
    while (1) {
        pthread_mutex_lock(&shm->mutex);
        if (shm->message_count > 0 && strcmp(last_message, shm->messages[shm->message_count - 1]) != 0) { // Проверка, изменилось ли последнее сообщение
            strcpy(last_message, shm->messages[shm->message_count - 1]); // Обновление последнего сообщения
            printf("New message: %s\n", last_message); // Вывод нового сообщения на сервере
        }
        pthread_mutex_unlock(&shm->mutex);
        sleep(1); // Ожидание перед следующей проверкой
    }
}

int main() {
    key_t key; // Ключ для сегмента разделяемой памяти
    int shmid; // Идентификатор сегмента разделяемой памяти
    struct shared_memory *shm; // Указатель на разделяемую память

    // Генерация уникального ключа
    key = ftok("server", 65);

    // Создание сегмента разделяемой памяти
    shmid = shmget(key, sizeof(struct shared_memory), 0666 | IPC_CREAT);
    if (shmid == -1) {
        perror("shmget");
        exit(1);
    }

    // Подключение к сегменту разделяемой памяти
    shm = (struct shared_memory *)shmat(shmid, NULL, 0);
    if (shm == (void *)-1) {
        perror("shmat");
        exit(1);
    }

    // Инициализация разделяемой памяти
    memset(shm, 0, sizeof(struct shared_memory));
    pthread_mutex_init(&shm->mutex, NULL);

    printf("Server started. Waiting for messages...\n");

    // Запуск потока для обработки сообщений
    pthread_t msg_thread;
    pthread_create(&msg_thread, NULL, (void *)handle_messages, shm);

    while (1) {
        // Ожидание новых сообщений или пользователей
        sleep(1);
    }

    // Отключение от сегмента разделяемой памяти
    shmdt(shm);

    // Удаление сегмента разделяемой памяти
    shmctl(shmid, IPC_RMID, NULL);

    return 0;
}