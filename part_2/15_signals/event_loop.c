#include <stdio.h>
#include <signal.h>
#include <unistd.h>

int main() {
    sigset_t set;
    int sig;

    // Блокируем SIGUSR1
    sigemptyset(&set);
    sigaddset(&set, SIGUSR1);
    sigprocmask(SIG_BLOCK, &set, NULL);

    printf("Waiting for SIGUSR1 signal. PID: %d\n", getpid());

    while (1) {
        sigwait(&set, &sig); // Ожидаем SIGUSR1
        printf("Received SIGUSR1 signal\n");
    }

    return 0;
}