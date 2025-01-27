#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <unistd.h>

#define SHM_SIZE 1024
#define SEM_KEY 5678  // Ключ для семафора

void sem_signal(int semid, int semnum) {
    struct sembuf sb = {semnum, 1, 0};
    if (semop(semid, &sb, 1) == -1) {
        perror("semop signal");
        exit(1);
    }
}

void sem_wait(int semid, int semnum) {
    struct sembuf sb = {semnum, -1, 0};
    if (semop(semid, &sb, 1) == -1) {
        perror("semop wait");
        exit(1);
    }
}

int main() {
    key_t key = 1234;  // Ключ для сегмента разделяемой памяти
    int shmid = shmget(key, SHM_SIZE, 0666 | IPC_CREAT);
    if (shmid < 0) {
        perror("shmget");
        exit(1);
    }

    char *str = (char*) shmat(shmid, (void*)0, 0);
    if (str == (char*) -1) {
        perror("shmat");
        exit(1);
    }

    // Подключение к семафору
    int semid = semget(SEM_KEY, 2, 0666);
    if (semid == -1) {
        perror("semget");
        exit(1);
    }

    // Ожидание сообщения от сервера
    sem_wait(semid, 0);

    printf("Клиент прочитал из памяти: %s\n", str);
    
    // Ответ серверу
    strcpy(str, "Hello!");
    printf("Отправили свое сообщение\n");

    // Сигнал серверу, что ответ отправлен
    sem_signal(semid, 1);
    
    // Отключение от сегмента разделяемой памяти
    if (shmdt(str) == -1) {
        perror("shmdt");
        exit(1);
    }

    return 0;
}