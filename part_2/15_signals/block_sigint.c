#include <stdio.h>
#include <signal.h>
#include <unistd.h>

int main() {
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, SIGINT);

    // Блокируем SIGINT
    sigprocmask(SIG_BLOCK, &set, NULL);

    printf("SIGINT is blocked. PID: %d\n", getpid());
    printf("Try sending SIGINT with 'kill -SIGINT %d'\n", getpid());

    while (1) {
        sleep(1);
    }

    return 0;
}