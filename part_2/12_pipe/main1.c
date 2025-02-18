#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    int fd[2];
    pid_t pid;
    char buffer[10] = {0}; // Инициализируем буфер нулями

    if (pipe(fd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid > 0) { // Родительский процесс
        close(fd[0]); // Закрываем дескриптор для чтения
        write(fd[1], "Hi!", 3);
        close(fd[1]); // Закрываем дескриптор для записи
        wait(NULL); // Ожидаем завершения дочернего процесса
    } else { // Дочерний процесс
        close(fd[1]); // Закрываем дескриптор для записи
        read(fd[0], buffer, 3); // Читаем только 3 байта
        buffer[3] = '\0'; // Завершаем строку нулевым символом
        printf("Received from parent: %s\n", buffer);
        close(fd[0]); // Закрываем дескриптор для чтения
    }

    return 0;
}