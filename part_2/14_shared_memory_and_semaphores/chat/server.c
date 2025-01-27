#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <pthread.h>
#include <unistd.h>
#include "shared_memory.h"

#define SEM_KEY 1234

int semid;

void sem_lock(int semid) {
    struct sembuf sb = {0, -1, 0};
    semop(semid, &sb, 1);
}

void sem_unlock(int semid) {
    struct sembuf sb = {0, 1, 0};
    semop(semid, &sb, 1);
}

void broadcast_message(struct shared_memory *shm, const char *message) {
    sem_lock(semid);
    if (shm->message_count < MAX_MESSAGES) {
        strcpy(shm->messages[shm->message_count], message);
        shm->message_count++;
    } else {
        for (int i = 1; i < MAX_MESSAGES; i++) {
            strcpy(shm->messages[i - 1], shm->messages[i]);
        }
        strcpy(shm->messages[MAX_MESSAGES - 1], message);
    }
    sem_unlock(semid);
    printf("Broadcast message: %s\n", message);
}

void add_user(struct shared_memory *shm, const char *name) {
    sem_lock(semid);
    if (shm->user_count < MAX_USERS) {
        strcpy(shm->users[shm->user_count], name);
        shm->user_count++;
        printf("User %s has connected.\n", name);

        char connect_msg[MAX_TEXT];
        snprintf(connect_msg, MAX_TEXT, "System: User %s has joined the chat.\n", name);
        broadcast_message(shm, connect_msg);
    }
    sem_unlock(semid);
}

void handle_messages(struct shared_memory *shm) {
    char last_message[MAX_TEXT] = "";
    while (1) {
        sem_lock(semid);
        if (shm->message_count > 0 && strcmp(last_message, shm->messages[shm->message_count - 1]) != 0) {
            strcpy(last_message, shm->messages[shm->message_count - 1]);
            printf("New message: %s\n", last_message);
        }
        sem_unlock(semid);
        sleep(1);
    }
}

int main() {
    key_t key;
    int shmid;
    struct shared_memory *shm;

    key = ftok("server", 65);
    shmid = shmget(key, sizeof(struct shared_memory), 0666 | IPC_CREAT);
    if (shmid == -1) {
        perror("shmget ");
        exit(1);
    }

    shm = (struct shared_memory *)shmat(shmid, NULL, 0);
    if (shm == (void *)-1) {
        perror("shmat");
        exit(1);
    }

    memset(shm, 0, sizeof(struct shared_memory));

    semid = semget(SEM_KEY, 1, 0666 | IPC_CREAT);
    semctl(semid, 0, SETVAL, 1);

    printf("Server started. Waiting for messages...\n");

    pthread_t msg_thread;
    pthread_create(&msg_thread, NULL, (void *)handle_messages, shm);

    while (1) {
        sleep(1);
    }

    shmdt(shm);
    shmctl(shmid, IPC_RMID, NULL);
    semctl(semid, 0, IPC_RMID);

    return 0;
}