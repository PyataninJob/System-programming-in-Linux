#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        exit(1);
    } else if (pid == 0) {
        // Дочерний
        printf("Child process: PID = %d, PPID = %d\n", getpid(), getppid());
        exit(0);
    } else {
        // Родительский
        int status;
        printf("Parent process: PID = %d, Child PID = %d\n", getpid(), pid);
        waitpid(pid, &status, 0); // ждем
        if (WIFEXITED(status)) {
            printf("Child exited with status = %d\n", WEXITSTATUS(status));
        } else {
            printf("Child did not exit successfully\n");
        }
    }

    return 0;
}