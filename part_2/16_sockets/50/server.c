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
#define DATA_LEN 46    // Длина данных (payload)

int main() {
    int sockfd;
    struct ifreq ifr;
    struct sockaddr_ll sa;
    unsigned char buffer[ETH_HDRLEN + DATA_LEN];

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

    // Заполнение MAC-адресов источника и назначения
    unsigned char dest_mac[6] = {0x00, 0x1A, 0x2B, 0x3C, 0x4D, 0x5E};
    memcpy(buffer, dest_mac, 6);
    memcpy(buffer + 6, ifr.ifr_hwaddr.sa_data, 6);

    // Заполнение поля sll_protocol
    buffer[12] = 0x88; // Тип Ethernet (0x88B5 для Ethertype)
    buffer[13] = 0xB5;

    // Заполнение данных (payload)
    const char *data = "Hello, this is a test message!";
    memcpy(buffer + ETH_HDRLEN, data, strlen(data));

    // Отправка данных
    if (sendto(sockfd, buffer, ETH_HDRLEN + strlen(data), 0, (struct sockaddr*)&sa, sizeof(struct sockaddr_ll)) == -1) {
        perror("sendto");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("Пакет отправлен успешно\n");

    close(sockfd);
    return 0;
}