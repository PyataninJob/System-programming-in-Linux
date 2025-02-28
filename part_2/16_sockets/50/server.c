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

#define INTERFACE "enp0s8" // Интерфейс сервера
#define SRC_MAC "08:00:27:bf:99:aa" // MAC-адрес сервера
#define CLIENT_IP "192.168.56.10"   // IP-адрес клиента

#define BUFFER_SIZE 4096            // Размер буфера

// Функция для вывода MAC-адреса
void print_mac(const unsigned char *mac) {
    printf("%02x:%02x:%02x:%02x:%02x:%02x\n",
           mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
}

// Функция для модификации сообщения
void modify_message(char *message) {
    // Проверка, чтобы избежать дублирования
    if (strncmp(message, "Modified: ", 10) != 0) {
        // Проверка, что в буфере достаточно места для модификации
        size_t message_len = strnlen(message, BUFFER_SIZE);
        if (message_len + 11 < BUFFER_SIZE) { // 11 = длина "Modified: " + 1 для '\0'
            // Добавляем префикс "Modified: "
            memmove(message + 10, message, message_len + 1); // +1 для '\0'
            memcpy(message, "Modified: ", 10);
        } else {
            printf("Buffer is too small to modify the message.\n");
        }
    }
}

// Функция для сравнения IP-адресов
int compare_ip(const char *ip1, const char *ip2) {
    return strcmp(ip1, ip2) == 0;
}

int main() {
    int sockfd;
    struct sockaddr_ll socket_address;
    socklen_t addr_len = sizeof(socket_address);
    char buffer[BUFFER_SIZE];

    // Создание RAW сокета
    if ((sockfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL))) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Получение индекса сетевого интерфейса с помощью if_nametoindex
    unsigned int if_index = if_nametoindex(INTERFACE);
    if (!if_index) {
        perror("Failed to get interface index");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // Настройка сетевого интерфейса
    socket_address.sll_family = AF_PACKET;
    socket_address.sll_ifindex = if_index;

    // Прием пакетов
    while (1) {
        int bytes_received = recvfrom(sockfd, buffer, BUFFER_SIZE, 0,
                                      (struct sockaddr *)&socket_address, &addr_len);
        if (bytes_received < 0) {
            perror("Receive failed");
            continue;
        }

        // Извлечение Ethernet-заголовка
        struct ether_header *ethhdr = (struct ether_header *)buffer;

        // Извлечение IP-заголовка
        struct iphdr *iphdr = (struct iphdr *)(buffer + sizeof(struct ether_header));

        // Преобразование IP-адреса источника в строковый формат
        char src_ip_str[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &(iphdr->saddr), src_ip_str, INET_ADDRSTRLEN);

        // Проверка, что пакет пришел от клиента
        if (!compare_ip(src_ip_str, CLIENT_IP)) {
            printf("Packet from unknown source (%s) ignored.\n", src_ip_str);
            continue; // Игнорируем пакет
        }
        

        // Вывод информации о полученном пакете
        printf("Received packet from:\n");
        printf("  Source MAC: ");
        print_mac(ethhdr->ether_shost);
        printf("  Destination MAC: ");
        print_mac(ethhdr->ether_dhost);
        printf("  Source IP: %s\n", src_ip_str);

        // Извлечение данных из пакета
        char *data = buffer + sizeof(struct ether_header) + sizeof(struct iphdr) + sizeof(struct udphdr);
        printf("Data: %s\n", data);

        // Модификация сообщения
        modify_message(data);
        printf("Modifi Data: %s\n", data);
        // Обновление длины UDP-заголовка
        struct udphdr *udphdr = (struct udphdr *)(buffer + sizeof(struct ether_header) + sizeof(struct iphdr));
        size_t new_data_len = strlen(data); // Длина модифицированного сообщения
        udphdr->len = htons(sizeof(struct udphdr) + new_data_len);
        // Обновление длины IP-заголовка
        iphdr->tot_len = htons(sizeof(struct iphdr) + sizeof(struct udphdr) + new_data_len);
        // Отправка модифицированного пакета обратно клиенту
        unsigned char src_mac[ETH_ALEN];
        sscanf(SRC_MAC, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx",
               &src_mac[0], &src_mac[1], &src_mac[2],
               &src_mac[3], &src_mac[4], &src_mac[5]);

        // Переключение MAC-адресов
        memcpy(ethhdr->ether_dhost, ethhdr->ether_shost, ETH_ALEN); // Новый MAC-адрес назначения
        memcpy(ethhdr->ether_shost, src_mac, ETH_ALEN);             // MAC-адрес источника
        printf("Sending packet with length: %d\n", bytes_received);
        // Отправка пакета
        size_t total_len = sizeof(struct ether_header) + ntohs(iphdr->tot_len);
        printf("Sending packet with length: %zu\n", total_len);
        
        if (sendto(sockfd, buffer, total_len, 0,
                   (struct sockaddr *)&socket_address, sizeof(struct sockaddr_ll)) < 0) {
            perror("Send failed");
            continue;
        }
        printf("Response sent successfully.\n");
    }

    close(sockfd);
    return 0;
}
