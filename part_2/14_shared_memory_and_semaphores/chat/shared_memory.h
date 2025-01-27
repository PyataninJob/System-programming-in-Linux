#ifndef SHARED_MEMORY_H
#define SHARED_MEMORY_H

#include <pthread.h>

#define SHM_SIZE 4096
#define MAX_TEXT 512
#define MAX_USERS 10
#define MAX_MESSAGES 100

struct shared_memory {
    char messages[MAX_MESSAGES][MAX_TEXT];
    char users[MAX_USERS][MAX_TEXT];
    int user_count;
    int message_count;
    pthread_mutex_t mutex;
};

#endif // SHARED_MEMORY_H