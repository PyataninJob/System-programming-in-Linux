#ifndef SHARED_MEMORY_H
#define SHARED_MEMORY_H

#include <pthread.h>

#define SHM_SIZE 4096 // Размер сегмента разделяемой памяти
#define MAX_TEXT 512 // Максимальный размер текста сообщения
#define MAX_USERS 10 // Максимальное количество пользователей
#define MAX_MESSAGES 100 // Максимальное количество сообщений

// Структура для разделяемой памяти
struct shared_memory {
    char messages[MAX_MESSAGES][MAX_TEXT]; // Буфер для сообщений
    char users[MAX_USERS][MAX_TEXT]; // Массив имен пользователей
    int user_count; // Количество пользователей
    int message_count; // Количество сообщений
    pthread_mutex_t mutex; // Мьютекс для синхронизации доступа
};

#endif // SHARED_MEMORY_H