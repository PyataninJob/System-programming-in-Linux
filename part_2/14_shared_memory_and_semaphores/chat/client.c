#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <pthread.h>
#include <ncurses.h>

#define SHM_KEY 12345
#define MAX_MSG_LEN 256
#define MAX_USERS 10

typedef struct {
    char messages[MAX_USERS][MAX_MSG_LEN];
    char users[MAX_USERS][MAX_MSG_LEN];
    int user_count;
    int msg_count;
} ChatRoom;

ChatRoom *chat_room;
char username[MAX_MSG_LEN];

WINDOW *msg_win, *user_win, *input_win, *msg_subwin;

void init_windows() {
    msg_win = newwin(LINES - 3, COLS - 21, 0, 0);
    box(msg_win, 0, 0);
    wrefresh(msg_win);

    msg_subwin = derwin(msg_win, LINES - 5, COLS - 23, 1, 1);
    scrollok(msg_subwin, TRUE);
    wrefresh(msg_subwin);

    user_win = newwin(LINES - 3, 20, 0, COLS - 20);
    box(user_win, 0, 0);
    mvwprintw(user_win, 1, 1, "Users:");
    wrefresh(user_win);

    input_win = newwin(3, COLS, LINES - 3, 0);
    box(input_win, 0, 0);
    mvwprintw(input_win, 1, 1, "Enter message:");
    wrefresh(input_win);
}

void update_message_window(const char *message) {
    int y, x;
    getyx(msg_subwin, y, x);
    if (y >= LINES - 5) {
        wscrl(msg_subwin, 1);
        y--;
    }
    mvwprintw(msg_subwin, y , 1, "%s\n", message);
    wrefresh(msg_subwin);
    box(msg_win, 0, 0); // Перерисовываем рамку после добавления сообщения
    wrefresh(msg_win);
}

void update_user_window() {
    werase(user_win);
    box(user_win, 0, 0);
    mvwprintw(user_win, 1, 1, "Users:");
    for (int i = 0; i < chat_room->user_count; i++) {
        mvwprintw(user_win, i + 2, 1, "%s", chat_room->users[i]);
    }
    wrefresh(user_win);
}

void update_input_window() {
    werase(input_win);
    box(input_win, 0, 0);
    mvwprintw(input_win, 1, 1, "Enter message:");
    wrefresh(input_win);
}

void *receive_messages(void *arg) {
    int last_msg_count = 0;
    while (1) {
        if (chat_room->msg_count > last_msg_count) {
            for (int i = last_msg_count; i < chat_room->msg_count; i++) {
                update_message_window(chat_room->messages[i]);
            }
            last_msg_count = chat_room->msg_count;
        }
        sleep(1);
    }
    return NULL;
}

void *receive_users(void *arg) {
    int last_user_count = 0;
    while (1) {
        if (chat_room->user_count > last_user_count) {
            update_user_window();
            last_user_count = chat_room->user_count;
        }
        sleep(1);
    }
    return NULL;
}

int main() {
    int shmid = shmget(SHM_KEY, sizeof(ChatRoom), 0666);
    if (shmid < 0) {
        perror("shmget");
        exit(1);
    }

    chat_room = (ChatRoom *)shmat(shmid, NULL, 0);
    if (chat_room == (ChatRoom *)-1) {
        perror("shmat");
        exit(1);
    }

    initscr();
    cbreak();
    noecho();

    init_windows();

    mvwprintw(input_win, 1, 1, "Enter your name: ");
    wrefresh(input_win);
    wgetstr(input_win, username);

    strcpy(chat_room->users[chat_room->user_count++], username);

    update_user_window();

    pthread_t msg_thread, user_thread;
    pthread_create(&msg_thread, NULL, receive_messages, NULL);
    pthread_create(&user_thread, NULL, receive_users, NULL);

    char message[MAX_MSG_LEN];
    while (1) {
        update_input_window();
        wgetstr(input_win, message);
        snprintf(chat_room->messages[chat_room->msg_count++], MAX_MSG_LEN, "%s: %s", username, message);
    }

    endwin();

    shmdt(chat_room);

    return 0;
}