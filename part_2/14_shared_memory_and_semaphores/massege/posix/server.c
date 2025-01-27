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
    char *str = (char*) shmat(shmid, (void*)0, 0);

    printf("Клиент прочитал из памяти: %s\n", str);
    
    // Responding to the server
    strcpy(str, "Hello!");
    printf("Отправили свое сообщение\n");
    
    
    // Отключаемся
    shmdt(str);

    return 0;
}