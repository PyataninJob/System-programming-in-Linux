#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <netinet/udp.h>

#define BUFFER_SIZE 65536

int main() {
    int sockfd;
    char buffer[BUFFER_SIZE];
    struct sockaddr_in addr;
    socklen_t addr_len = sizeof(addr);

    // Создание RAW-сокета
    sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
    if (sockfd < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    printf("Listening for UDP packets...\n");

    // Бесконечный цикл для получения данных
    while (1) {
        ssize_t data_len = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&addr, &addr_len);
        if (data_len < 0) {
            perror("recvfrom");
            close(sockfd);
            exit(EXIT_FAILURE);
        }

        struct iphdr *ip_header = (struct iphdr *)buffer;
        struct udphdr *udp_header = (struct udphdr *)(buffer + (ip_header->ihl * 4));

        printf("Received packet from %s:%d\n", inet_ntoa(*(struct in_addr *)&ip_header->saddr), ntohs(udp_header->source));
        printf("Data: ");
        for (int i = sizeof(struct iphdr) + sizeof(struct udphdr); i < data_len; i++) {
            printf("%02x ", (unsigned char)buffer[i]);
        }
        printf("\n");
    }

    close(sockfd);
    return 0;
}