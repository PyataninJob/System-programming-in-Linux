// System V Client
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

#define SHM_SIZE 1024

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

    printf("Клиент прочитал из памяти: %s\n", str);
    
    // Ответ серверу
    strcpy(str, "Hello!");
    printf("Отправили свое сообщение\n");
    
    // Отключаемся
    if (shmdt(str) == -1) {
        perror("shmdt");
        exit(1);
    }

    return 0;
}