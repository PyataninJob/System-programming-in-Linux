#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <pthread.h>

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

void *notify_clients(void *arg) {
    int last_user_count = 0;
    while (1) {
        sleep(1);
        if (chat_room->user_count > last_user_count) {
            for (int i = last_user_count; i < chat_room->user_count; i++) {
                snprintf(chat_room->messages[chat_room->msg_count++], MAX_MSG_LEN, "User %s has joined the chat", chat_room->users[i]);
            }
            last_user_count = chat_room->user_count;
        }
        // Notify clients about new messages and users
    }
    return NULL;
}

int main() {
    int shmid = shmget(SHM_KEY, sizeof(ChatRoom), IPC_CREAT | 0666);
    if (shmid < 0) {
        perror("shmget");
        exit(1);
    }

    chat_room = (ChatRoom *)shmat(shmid, NULL, 0);
    if (chat_room == (ChatRoom *)-1) {
        perror("shmat");
        exit(1);
    }

    chat_room->user_count = 0;
    chat_room->msg_count = 0;

    pthread_t notify_thread;
    pthread_create(&notify_thread, NULL, notify_clients, NULL);

    printf("Server started. Waiting for clients...\n");

    pthread_join(notify_thread, NULL);

    shmdt(chat_room);
    shmctl(shmid, IPC_RMID, NULL);

    return 0;
}