#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

#define MAX_INPUT 1024
#define MAX_ARGS 64

void execute_command(char *input);
void parse_input(char *input, char **args);
void execute_pipeline(char **commands, int num_commands);

int main() {
    char input[MAX_INPUT];

    while (1) {
        printf("bash> ");
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        execute_command(input);
    }

    return 0;
}

void execute_command(char *input) {
    char *commands[MAX_ARGS];
    int num_commands = 0;

    char *token = strtok(input, "|");
    while (token != NULL && num_commands < MAX_ARGS - 1) {
        commands[num_commands++] = token;
        token = strtok(NULL, "|");
    }
    commands[num_commands] = NULL;

    if (num_commands == 1) {
        char *args[MAX_ARGS];
        parse_input(commands[0], args);

        if (args[0] == NULL) {
            return;
        }

        if (strcmp(args[0], "exit") == 0) {
            exit(0);
        }

        pid_t pid = fork();
        if (pid < 0) {
            perror("fork");
            exit(1);
        } else if (pid == 0) {
            execvp(args[0], args);
            perror("execvp");
            exit(1);
        } else {
            int status;
            waitpid(pid, &status, 0);
            if (WIFEXITED(status)) {
                printf("Child exited with status = %d\n", WEXITSTATUS(status));
            } else {
                printf("Child did not exit successfully\n");
            }
        }
    } else {
        execute_pipeline(commands, num_commands);
    }
}

void parse_input(char *input, char **args) {
    char *token = strtok(input, " \n");
    int i = 0;

    while (token != NULL && i < MAX_ARGS - 1) {
        args[i++] = token;
        token = strtok(NULL, " \n");
    }
    args[i] = NULL;
}

void execute_pipeline(char **commands, int num_commands) {
    int pipefd[2];
    pid_t pid;
    int fd_in = 0;

    for (int i = 0; i < num_commands; i++) {
        pipe(pipefd);
        if ((pid = fork()) == -1) {
            perror("fork");
            exit(1);
        } else if (pid == 0) {
            dup2(fd_in, 0);
            if (i < num_commands - 1) {
                dup2(pipefd[1], 1);
            }
            close(pipefd[0]);

            char *args[MAX_ARGS];
            parse_input(commands[i], args);
            execvp(args[0], args);
            perror("execvp");
            exit(1);
        } else {
            wait(NULL);
            close(pipefd[1]);
            fd_in = pipefd[0];
        }
    }
}