#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <unistd.h>

#define SHM_SIZE 1024  // Размер сегмента разделяемой памяти
#define SEM_KEY 5678   // Ключ для семафора

union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

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
    int shmid, semid;
    key_t key = 1234;  // Ключ для сегмента разделяемой памяти
    char *shm;

    // Создание сегмента разделяемой памяти
    shmid = shmget(key, SHM_SIZE, IPC_CREAT | 0666);
    if (shmid < 0) {
        perror("shmget");
        exit(1);
    }

    // Подключение к сегменту разделяемой памяти
    shm = shmat(shmid, NULL, 0);
    if (shm == (char *) -1) {
        perror("shmat");
        exit(1);
    }

    // Создание семафора
    semid = semget(SEM_KEY, 2, IPC_CREAT | 0666);
    if (semid == -1) {
        perror("semget");
        exit(1);
    }

    // Инициализация семафоров
    union semun sem_union;
    sem_union.val = 0;
    if (semctl(semid, 0, SETVAL, sem_union) == -1) {
        perror("semctl setval");
        exit(1);
    }
    sem_union.val = 0;
    if (semctl(semid, 1, SETVAL, sem_union) == -1) {
        perror("semctl setval");
        exit(1);
    }

    // Запись сообщения в сегмент разделяемой памяти
    strcpy(shm, "Hi!");
    printf("Отправили свое сообщение\n");

    // Сигнал клиенту, что сообщение отправлено
    sem_signal(semid, 0);

    // Ожидание ответа от клиента
    printf("Ожидание ответа от клиента...\n");
    sem_wait(semid, 1);

    // Вывод ответа клиента
    printf("Ответ клиента: %s\n", shm);

    // Отключение от сегмента разделяемой памяти
    if (shmdt(shm) == -1) {
        perror("shmdt");
        exit(1);
    }

    // Удаление сегмента разделяемой памяти
    if (shmctl(shmid, IPC_RMID, NULL) == -1) {
        perror("shmctl");
        exit(1);
    }

    // Удаление семафоров
    if (semctl(semid, 0, IPC_RMID) == -1) {
        perror("semctl remove");
        exit(1);
    }

    return 0;
}