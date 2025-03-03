#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <net/if.h> // Для if_nametoindex
#include <netinet/ether.h>
#include <linux/if_packet.h>
#include <netinet/ip.h> // Для struct iphdr
#include <netinet/udp.h> // Для struct udphdr>

#define INTERFACE "enp0s8" // Интерфейс клиента (проверить командой `ip a`)
#define DEST_MAC "08:00:27:bf:99:aa" // MAC-адрес сервера
#define SRC_PORT 12345              // Порт источника
#define DST_PORT 8080               // Порт назначения
#define ETHER_TYPE 0x0800           // Тип Ethernet-кадра (IPv4)
#define BUFFER_SIZE 4096            // Размер буфера

#define SRC_IP "192.168.56.10"      // IP-адрес клиента
#define DST_IP "192.168.56.11"      // IP-адрес сервера

// Функция для вывода MAC-адреса
void print_mac(const unsigned char *mac) {
    printf("%02x:%02x:%02x:%02x:%02x:%02x\n",
           mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
}

// Функция для получения MAC-адреса интерфейса из /sys/class/net/<interface>/address
void get_mac_address(const char *interface, unsigned char mac[ETH_ALEN]) {
    char path[64];
    snprintf(path, sizeof(path), "/sys/class/net/%s/address", interface);

    FILE *fp = fopen(path, "r");
    if (!fp) {
        perror("Failed to open MAC address file");
        exit(EXIT_FAILURE);
    }

    if (fscanf(fp, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx",
               &mac[0], &mac[1], &mac[2],
               &mac[3], &mac[4], &mac[5]) != 6) {
        perror("Failed to read MAC address");
        fclose(fp);
        exit(EXIT_FAILURE);
    }

    fclose(fp);
}

int main() {
    int sockfd;
    int tx_len = 0, rx_len = 0;
    char sendbuf[BUFFER_SIZE], recvbuf[BUFFER_SIZE];

    // Создание RAW сокета
    if ((sockfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL))) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Получение MAC-адреса источника
    unsigned char src_mac[ETH_ALEN];
    get_mac_address(INTERFACE, src_mac);

    // Преобразование MAC-адреса назначения
    unsigned char dest_mac[ETH_ALEN];
    sscanf(DEST_MAC, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx",
           &dest_mac[0], &dest_mac[1], &dest_mac[2],
           &dest_mac[3], &dest_mac[4], &dest_mac[5]);

    // Получение индекса сетевого интерфейса с помощью if_nametoindex
    unsigned int if_index = if_nametoindex(INTERFACE);
    if (!if_index) {
        perror("Failed to get interface index");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // Заполнение заголовка канального уровня (Ethernet)
    struct sockaddr_ll socket_address;
    memset(&socket_address, 0, sizeof(socket_address));
    socket_address.sll_family = AF_PACKET;          // Семейство адресов
    socket_address.sll_protocol = htons(ETHER_TYPE); // Тип Ethernet-кадра (IPv4)
    socket_address.sll_ifindex = if_index;          // Индекс сетевого интерфейса
    socket_address.sll_hatype = ARPHRD_ETHER;       // Тип аппаратного адреса (Ethernet)
    socket_address.sll_pkttype = PACKET_OTHERHOST;  // Тип пакета
    socket_address.sll_halen = ETH_ALEN;            // Длина MAC-адреса
    memcpy(socket_address.sll_addr, dest_mac, ETH_ALEN); // MAC-адрес назначения

    // Формирование Ethernet-заголовка
    struct ether_header *ethhdr = (struct ether_header *)sendbuf;
    memcpy(ethhdr->ether_dhost, dest_mac, ETH_ALEN); // MAC-адрес назначения
    memcpy(ethhdr->ether_shost, src_mac, ETH_ALEN);  // MAC-адрес источника
    ethhdr->ether_type = htons(ETHER_TYPE);         // Тип Ethernet-кадра (IPv4)

    // Заполнение IP-заголовка
    struct iphdr *iphdr = (struct iphdr *)(sendbuf + sizeof(struct ether_header));
    iphdr->version = 4;                          // Версия IP (IPv4)
    iphdr->ihl = 5;                              // Длина заголовка (5 * 4 байта)
    iphdr->tos = 0;                              // Тип сервиса
    iphdr->tot_len = htons(sizeof(struct iphdr) + sizeof(struct udphdr) + strlen("Hello, Server!")); // Общая длина пакета
    iphdr->id = htons(0);                        // Идентификатор пакета
    iphdr->frag_off = 0;                         // Флаги фрагментации
    iphdr->ttl = 64;                             // Время жизни пакета
    iphdr->protocol = IPPROTO_UDP;               // Протокол (UDP)
    iphdr->check = 0;                            // Контрольная сумма (вычисляется автоматически)
    iphdr->saddr = inet_addr(SRC_IP);            // IP-адрес источника
    iphdr->daddr = inet_addr(DST_IP);            // IP-адрес назначения

    // Заполнение UDP-заголовка
    struct udphdr *udphdr = (struct udphdr *)(sendbuf + sizeof(struct ether_header) + sizeof(struct iphdr));
    udphdr->source = htons(SRC_PORT);            // Порт источника
    udphdr->dest = htons(DST_PORT);              // Порт назначения
    udphdr->len = htons(sizeof(struct udphdr) + strlen("Hello, Server!")); // Длина UDP-данных
    udphdr->check = 0;                           // Контрольная сумма (не используется)

    // Добавление данных после заголовков
    const char *message = "Hello, Server!";
    memcpy(sendbuf + sizeof(struct ether_header) + sizeof(struct iphdr) + sizeof(struct udphdr), message, strlen(message));

    // Вычисление общей длины пакета
    tx_len = sizeof(struct ether_header) + sizeof(struct iphdr) + sizeof(struct udphdr) + strlen(message);

    // Отправка пакета
    if (sendto(sockfd, sendbuf, tx_len, 0, (struct sockaddr *)&socket_address, sizeof(struct sockaddr_ll)) < 0) {
        perror("Send failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("Packet sent successfully.\n");

    // Прием ответа от сервера
    socklen_t addr_len = sizeof(socket_address);
    memset(recvbuf, 0, BUFFER_SIZE); // Очистка буфера перед чтением
    rx_len = recvfrom(sockfd, recvbuf, BUFFER_SIZE, 0, (struct sockaddr *)&socket_address, &addr_len);
    if (rx_len < 0) {
        perror("Receive failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    printf("Received packet with length: %d\n", rx_len);

    // Извлечение заголовков
    struct ether_header *recv_ethhdr = (struct ether_header *)recvbuf;
    struct iphdr *recv_iphdr = (struct iphdr *)(recvbuf + sizeof(struct ether_header));
    size_t ip_header_len = recv_iphdr->ihl * 4; // Размер IP-заголовка
    struct udphdr *recv_udphdr = (struct udphdr *)(recvbuf + sizeof(struct ether_header) + ip_header_len);
    printf("Full packet (%d bytes):\n", rx_len);
    for (int i = 0; i < rx_len; i++) {
        printf("%02x ", (unsigned char)recvbuf[i]);
        if ((i + 1) % 16 == 0) printf("\n");
    }
    printf("\n");
    // Проверка типа Ethernet-кадра
    if (ntohs(recv_ethhdr->ether_type) != ETHER_TYPE) {
        printf("Received non-IPv4 packet.\n");
        close(sockfd);
        return 0;
    }

    // Проверка протокола IP
    if (recv_iphdr->protocol != IPPROTO_UDP) {
        printf("Received non-UDP packet.\n");
        close(sockfd);
        return 0;
    }

    // Длина данных из UDP-заголовка
    size_t udp_data_len = ntohs(recv_udphdr->len) - sizeof(struct udphdr);

    // Извлечение данных
    const char *data = recvbuf + sizeof(struct ether_header) + ip_header_len + sizeof(struct udphdr);

    // Вывод данных
    if (udp_data_len > 0 && udp_data_len < BUFFER_SIZE) {
        printf("Received response (%zu bytes): %.*s\n", udp_data_len, (int)udp_data_len, data);
    } else {
        printf("Received incomplete or invalid response.\n");
    }
    close(sockfd);
    return 0;
}