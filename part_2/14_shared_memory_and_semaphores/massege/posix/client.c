// POSIX Client
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

#define SHM_NAME "/my_shm"
#define SHM_SIZE 256

int main() {
    int fd;
    char *ptr;

    // Подключение к сегменту разделяемой памяти
    fd = shm_open(SHM_NAME, O_RDWR, 0666);
    if (fd == -1) {
        perror("shm_open");
        exit(1);
    }

    ptr = mmap(0, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (ptr == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }

    // Чтение сообщения от сервера
    printf("Сообщение от сервера: %s\n", ptr);

    // Ответ серверу
    const char *response = "Hello!";
    printf("Отправка ответа серверу: %s\n", response);
    strcpy(ptr, response);

    // Освобождение ресурсов
    if (munmap(ptr, SHM_SIZE) == -1) {
        perror("munmap");
        exit(1);
    }

    if (close(fd) == -1) {
        perror("close");
        exit(1);
    }

    return 0;
}