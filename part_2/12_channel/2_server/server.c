#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#define FIFO_NAME "/tmp/my_fifo"

int main() {
    int fd;

    // Удаляем существующий именованный канал, если он есть
    unlink(FIFO_NAME);

    // Создаем именованный канал
    if (mkfifo(FIFO_NAME, 0666) == -1) {
        perror("mkfifo");
        exit(EXIT_FAILURE);
    }

    // Открываем канал только на запись
    fd = open(FIFO_NAME, O_WRONLY);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    // Записываем строку в канал
    write(fd, "Hi!", 3);
    close(fd); // Закрываем дескриптор

    return 0;
}