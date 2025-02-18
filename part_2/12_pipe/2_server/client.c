#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define FIFO_NAME "/tmp/my_fifo"

int main() {
    int fd;
    char buffer[10];

    // Открываем канал только на чтение
    fd = open(FIFO_NAME, O_RDONLY);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    // Считываем строку из канала
    int num_bytes = read(fd, buffer, sizeof(buffer) - 1);
    if (num_bytes == -1) {
        perror("read");
        close(fd);
        exit(EXIT_FAILURE);
    }
    buffer[num_bytes] = '\0'; // Добавляем символ конца строки

    printf("Received from server: %s\n", buffer);
    close(fd); // Закрываем дескриптор

    // Удаляем именованный канал
    unlink(FIFO_NAME);

    return 0;
}