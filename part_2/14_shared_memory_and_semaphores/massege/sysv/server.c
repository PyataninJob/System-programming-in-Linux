// System V Server
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>

#define SHM_SIZE 1024  // Размер сегмента разделяемой памяти

int main() {
    int shmid;
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

    // Запись сообщения в сегмент разделяемой памяти
    strcpy(shm, "Hi!");
    printf("Отправили свое сообщение\n");

    // Ожидание ответа от клиента
    printf("Ожидание ответа от клиента...\n");
    while (strcmp(shm, "Hello!") != 0) {
        sleep(1);  // Ждем, пока клиент не ответит
    }

    // Вывод ответа клиента
    printf("Ответ клиента: %s\n", shm);

    // Удаление сегмента разделяемой памяти
    if (shmdt(shm) == -1) {
        perror("shmdt");
        exit(1);
    }

    if (shmctl(shmid, IPC_RMID, NULL) == -1) {
        perror("shmctl");
        exit(1);
    }

    return 0;
}