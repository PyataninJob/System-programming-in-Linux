#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>   // Для системных вызовов, таких как fork() и execvp()
#include <sys/wait.h> // Для функции waitpid() и макросов для анализа статуса завершения процесса
#include <string.h>

#define MAX_INPUT 1024
#define MAX_ARGS 64

void execute_command(char *input);
void parse_input(char *input, char **args);

int main() {
    char input[MAX_INPUT]; // Буфер для ввода команды

    while (1) {
        printf("bash> "); // Выводит приглашение для ввода команды
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break; // Если ввод завершен, выйти из цикла
        }
        execute_command(input); // Выполняет введенную команду
    }

    return 0; 
}

void execute_command(char *input) {
    char *args[MAX_ARGS];
    parse_input(input, args); // Разбирает ввод и заполняет массив аргументов

    if (args[0] == NULL) {
        return;
    }

    if (strcmp(args[0], "exit") == 0) {
        exit(0); 
    }

    pid_t pid = fork(); // Создает новый процесс
    if (pid < 0) {
        perror("fork");
        exit(1); 
    } else if (pid == 0) {
        // Дочерний процесс
        execvp(args[0], args); // Выполняет команду
        perror("execvp");
        exit(1);
    } else {
        // Родительский процесс
        int status;
        waitpid(pid, &status, 0); // Ждем
        if (WIFEXITED(status)) {
            printf("Child exited with status = %d\n", WEXITSTATUS(status)); // Выводит статус завершения дочернего процесса
        } else {
            printf("Child did not exit successfully\n");
        }
    }
}

void parse_input(char *input, char **args) {
    char *token = strtok(input, " \n"); // Разделяет строку на токены по пробелам и символам новой строки
    int i = 0;

    while (token != NULL && i < MAX_ARGS - 1) {
        args[i++] = token; // Заполняет массив аргументов
        token = strtok(NULL, " \n"); // Получает следующий токен
    }
    args[i] = NULL; // Завершает массив аргументов NULL
}