#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8080
#define BUFFER_SIZE 1024
#define HEADER_SIZE 8

void modify_message(char *message) {
    char modified[1024];
    snprintf(modified, sizeof(modified), "Modified: %s", message);
    strncpy(message, modified, BUFFER_SIZE - 1);
}

int main() {
    int sockfd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(sockfd, (const struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("Server is running on port %d...\n", PORT);

    while (1) {
        char buffer[BUFFER_SIZE];
        ssize_t bytes_received = recvfrom(sockfd, buffer, BUFFER_SIZE, 0,
                                          (struct sockaddr *)&client_addr, &addr_len);
        if (bytes_received > HEADER_SIZE) {
            buffer[bytes_received] = '\0';
            printf("Received message from client: %s\n", buffer + HEADER_SIZE);

            modify_message(buffer + HEADER_SIZE);

            size_t data_length = strlen(buffer + HEADER_SIZE);
            size_t total_length = HEADER_SIZE + data_length;

            if (sendto(sockfd, buffer, total_length, 0,
                       (struct sockaddr *)&client_addr, addr_len) < 0) {
                perror("Sendto failed");
            } else {
                printf("Sent modified message back to client.\n");
            }
        } else {
            printf("Received packet too small to contain data.\n");
        }
    }

    close(sockfd);
    return 0;
}