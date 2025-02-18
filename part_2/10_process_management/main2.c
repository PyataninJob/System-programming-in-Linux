#include <stdio.h>    // Для функций ввода-вывода
#include <stdlib.h>   // Для функций стандартной библиотеки, таких как exit()
#include <unistd.h>   // Для системных вызовов, таких как fork() и getpid()
#include <sys/wait.h> // Для функции wait() и макросов для анализа статуса завершения процесса
#include <string.h>   // Для работы со строками, таких как strcmp()

void handle_child_process(const char *child_name);
void wait_for_children(const char *parent_name, int num_children);
void create_child_process(const char *child_name);
void create_child_processes(const char *parent_name, const char *child_names[], int num_children);

int main() {
    printf("Parent process: PID = %d\n", getpid()); // Выводит PID родительского процесса
    const char *children[] = {"Process1", "Process2"};
    create_child_processes("Parent", children, 2); // Порождает процесс1 и процесс2
    return 0; // Завершает родительский процесс
}

void create_child_processes(const char *parent_name, const char *child_names[], int num_children) {
    for (int i = 0; i < num_children; i++) {
        create_child_process(child_names[i]); // Создает и обрабатывает дочерний процесс
    }

    wait_for_children(parent_name, num_children); // Ждет завершения всех дочерних процессов
}

void create_child_process(const char *child_name) {
    pid_t pid = fork(); // Создает дочерний процесс
    if (pid < 0) {
        perror("fork");
        exit(1); 
    } else if (pid == 0) {
        handle_child_process(child_name); // Обрабатывает дочерний процесс
    }
}

void handle_child_process(const char *child_name) {
    printf("%s: PID = %d, PPID = %d\n", child_name, getpid(), getppid()); // Выводит имя, PID и PPID дочернего процесса
    if (strcmp(child_name, "Process1") == 0) {
        // Процесс1 порождает процесс3 и процесс4
        const char *children[] = {"Process3", "Process4"};
        create_child_processes("Process1", children, 2);
    } else if (strcmp(child_name, "Process2") == 0) {
        // Процесс2 порождает процесс5
        const char *children[] = {"Process5"};
        create_child_processes("Process2", children, 1);
    }
    exit(0); // Завершает дочерний процесс
}

void wait_for_children(const char *parent_name, int num_children) {
    for (int i = 0; i < num_children; i++) {
        int status;
        pid_t pid = wait(&status); // Ждет завершения любого дочернего процесса и получает его статус
        if (pid > 0) {// Выводит имя родительского процесса, PID и статус завершения дочернего процесса
            printf("%s: Child with PID = %d exited with status = %d\n", parent_name, pid, WEXITSTATUS(status)); 
        }
    }
}

