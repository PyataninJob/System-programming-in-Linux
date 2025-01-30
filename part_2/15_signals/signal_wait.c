#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void handle_signal(int sig) {
    printf("Received SIGUSR1 signal\n");
}

int main() {
    struct sigaction sa;
    sa.sa_handler = handle_signal;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    // Устанавливаем обработчик для SIGUSR1
    sigaction(SIGUSR1, &sa, NULL);

    printf("Waiting for SIGUSR1 signal. PID: %d\n", getpid());
    while (1) {
        pause(); // Ожидаем сигнал
    }

    return 0;
}