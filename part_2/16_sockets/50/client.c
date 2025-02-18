#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/if_ether.h>
#include <netpacket/packet.h>
#include <net/if.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>

#define ETH_HDRLEN 14  // Длина заголовка Ethernet
#define BUFFER_SIZE 1500  // Размер буфера для приема данных

int main() {
    int sockfd;
    struct ifreq ifr;
    struct sockaddr_ll sa;
    unsigned char buffer[BUFFER_SIZE];

    // Создание сырого сокета
    if ((sockfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL))) == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Получение MAC-адреса интерфейса
    strncpy(ifr.ifr_name, "enp0s3", IFNAMSIZ-1);
    if (ioctl(sockfd, SIOCGIFHWADDR, &ifr) == -1) {
        perror("ioctl");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // Заполнение структуры sockaddr_ll
    memset(&sa, 0, sizeof(struct sockaddr_ll));
    sa.sll_family = AF_PACKET;
    sa.sll_protocol = htons(ETH_P_ALL);
    sa.sll_ifindex = if_nametoindex("enp0s3");
    sa.sll_halen = ETH_ALEN;
    memcpy(sa.sll_addr, ifr.ifr_hwaddr.sa_data, ETH_ALEN);

    // Привязка сокета к интерфейсу
    if (bind(sockfd, (struct sockaddr*)&sa, sizeof(struct sockaddr_ll)) == -1) {
        perror("bind");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // Прием данных
    while (1) {
        ssize_t numbytes = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, NULL, NULL);
        if (numbytes == -1) {
            perror("recvfrom");
            close(sockfd);
            exit(EXIT_FAILURE);
        }

        printf("Получен пакет длиной %zd байт\n", numbytes);

        // Проверка MAC-адресов и типа Ethernet
        if (memcmp(buffer, "\x00\x1A\x2B\x3C\x4D\x5E", 6) == 0 && memcmp(buffer + 6, ifr.ifr_hwaddr.sa_data, 6) == 0 && buffer[12] == 0x88 && buffer[13] == 0xB5) {
            printf("Пакет от источника 08:00:27:d2:82:46 к назначению 00:1A:2B:3C:4D:5E с типом 0x88B5\n");
            printf("Данные: %s\n", buffer + ETH_HDRLEN);
        }
    }

    close(sockfd);
    return 0;
}