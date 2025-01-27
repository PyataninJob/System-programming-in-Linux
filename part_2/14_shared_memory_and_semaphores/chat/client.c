#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <pthread.h>
#include <ncurses.h>
#include <unistd.h>
#include "shared_memory.h"

#define SEM_KEY 1234

WINDOW *messages_win, *users_win, *input_win;
char username[MAX_TEXT];
struct shared_memory *shm;
int semid;

void sem_lock(int semid) {
    struct sembuf sb = {0, -1, 0};
    semop(semid, &sb, 1);
}

void sem_unlock(int semid) {
    struct sembuf sb = {0, 1, 0};
    semop(semid, &sb, 1);
}

void *receive_messages(void *arg) {
    int last_message_count = 0;
    while (1) {
        sem_lock(semid);
        if (shm->message_count != last_message_count) {
            last_message_count = shm->message_count;
            werase(messages_win);
            int start = shm->message_count > 10 ? shm->message_count - 10 : 0;
            for (int i = start; i < shm->message_count; i++) {
                mvwprintw(messages_win, i - start + 1, 1, "%s", shm->messages[i]);
            }
            box(messages_win, 0, 0);
            wrefresh(messages_win);
        }
        sem_unlock(semid);
        sleep(1);
    }
    return NULL;
}

void init_windows() {
    int height, width;
    getmaxyx(stdscr, height, width);

    messages_win = newwin(height - 3, width - 20, 0, 0);
    users_win = newwin(height - 3, 20, 0, width - 20);
    input_win = newwin(3, width, height - 3, 0);

    box(messages_win, 0, 0);
    box(users_win, 0, 0);
    box(input_win, 0, 0);

    mvwprintw(users_win, 1, 1, "Users:");
    wrefresh(messages_win);
    wrefresh(users_win);
    wrefresh(input_win);
}

int main() {
    key_t key;
    int shmid;
    pthread_t recv_thread;

    key = ftok("server", 65);
    shmid = shmget(key, sizeof(struct shared_memory), 0666);
    if (shmid == -1) {
        perror("shmget");
        exit(1);
    }

    shm = (struct shared_memory *)shmat(shmid, NULL, 0);
    if (shm == (void *)-1) {
        perror("shmat");
        exit(1);
    }

    semid = semget(SEM_KEY, 1, 0666);

    initscr();
    cbreak();
    echo();
    keypad(stdscr, TRUE);

    init_windows();

    pthread_create(&recv_thread, NULL, receive_messages, NULL);

    mvwprintw(input_win, 1, 1, "Enter your name: ");
    wrefresh(input_win);
    wgetnstr(input_win, username, MAX_TEXT);

    sem_lock(semid);
    if (shm->user_count < MAX_USERS) {
        strcpy(shm->users[shm->user_count], username);
        shm->user_count++;
    }
    sem_unlock(semid);

    char connect_msg[MAX_TEXT];
    snprintf(connect_msg, MAX_TEXT, "System: User %.511s has joined the chat.\n", username);
    sem_lock(semid);
    if (shm->message_count < MAX_MESSAGES) {
        strcpy(shm->messages[shm->message_count], connect_msg);
        shm->message_count++;
    } else {
        for (int i = 1; i < MAX_MESSAGES; i++) {
            strcpy(shm->messages[i - 1], shm->messages[i]);
        }
        strcpy(shm->messages[MAX_MESSAGES - 1], connect_msg);
    }
    sem_unlock(semid);

    while (1) {
        werase(input_win);
        box(input_win, 0, 0);
        mvwprintw(input_win, 1, 1, "Enter message: ");
        wrefresh(input_win);

        char input[MAX_TEXT];
        wgetnstr(input_win, input, MAX_TEXT);

        char full_msg[MAX_TEXT];
        snprintf(full_msg, MAX_TEXT, "%.250s: %.250s\n", username, input);

        sem_lock(semid);
        if (shm->message_count < MAX_MESSAGES) {
            strcpy(shm->messages[shm->message_count], full_msg);
            shm->message_count++;
        } else {
            for (int i = 1; i < MAX_MESSAGES; i++) {
                strcpy(shm->messages[i - 1], shm->messages[i]);
            }
            strcpy(shm->messages[MAX_MESSAGES - 1], full_msg);
        }
        sem_unlock(semid);
    }

    endwin();
    return 0;
}