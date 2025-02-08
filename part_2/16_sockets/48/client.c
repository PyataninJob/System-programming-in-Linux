#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netinet/udp.h>

#define PORT 8080
#define BUFFER_SIZE 1024

void print_udp_header(struct udphdr *udph) {
    printf("UDP Header:\n");
    printf("  Source Port: %d\n", ntohs(udph->source));
    printf("  Destination Port: %d\n", ntohs(udph->dest));
    printf("  Length: %d\n", ntohs(udph->len));
    printf("  Checksum: %d\n", ntohs(udph->check));
}

void print_ip_header(struct iphdr *iph) {
    printf("IP Header:\n");
    printf("  Version: %d\n", iph->version);
    printf("  Header Length: %d\n", iph->ihl);
    printf("  Total Length: %d\n", ntohs(iph->tot_len));
    printf("  Protocol: %d\n", iph->protocol);
    printf("  Source IP: %s\n", inet_ntoa(*(struct in_addr *)&iph->saddr));
    printf("  Destination IP: %s\n", inet_ntoa(*(struct in_addr *)&iph->daddr));
}

void print_data(const char *label, const char *data, int length) {
    printf("%s: ", label);
    for (int i = 0; i < length; i++) {
        printf("%02x ", (unsigned char)data[i]);
    }
    printf("\n");
}

int main() {
    int sockfd;
    char buffer[BUFFER_SIZE];
    char *message = "Привет, сервер";
    struct sockaddr_in servaddr, recvaddr;
    socklen_t recvaddr_len = sizeof(recvaddr);

    // Создание RAW сокета
    if ((sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_UDP)) < 0) {
        perror("Ошибка создания сокета");
        exit(EXIT_FAILURE);
    }
    printf("Сокет создан.\n");

    // Включение опции IP_HDRINCL
    int one = 1;
    if (setsockopt(sockfd, IPPROTO_IP, IP_HDRINCL, &one, sizeof(one)) < 0) {
        perror("Ошибка установки IP_HDRINCL");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Формирование пакета
    char packet[BUFFER_SIZE];
    struct iphdr *iph = (struct iphdr *)packet;
    struct udphdr *udph = (struct udphdr *)(packet + sizeof(struct iphdr));
    char *data = packet + sizeof(struct iphdr) + sizeof(struct udphdr);
    strcpy(data, message);

    // Заполнение заголовка IP
    iph->ihl = 5;
    iph->version = 4;
    iph->tos = 0;
    iph->tot_len = htons(sizeof(struct iphdr) + sizeof(struct udphdr) + strlen(message));
    iph->id = 0;
    iph->frag_off = 0;
    iph->ttl = 255;
    iph->protocol = IPPROTO_UDP;
    iph->check = 0;
    iph->saddr = inet_addr("127.0.0.1");
    iph->daddr = servaddr.sin_addr.s_addr;

    // Заполнение заголовка UDP
    udph->source = htons(12345); // произвольный порт источника
    udph->dest = htons(PORT);
    udph->len = htons(sizeof(struct udphdr) + strlen(message));
    udph->check = 0;

    // Вывод заголовков IP и UDP перед отправкой
    print_ip_header(iph);
    print_udp_header(udph);

    // Вывод данных перед отправкой
    print_data("Отправляемые данные", data, strlen(message));

    // Отправка пакета серверу
    if (sendto(sockfd, packet, ntohs(iph->tot_len), 0, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("Ошибка отправки пакета");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    printf("Сообщение отправлено: %s\n", message);

    // Получение ответа от сервера
    int n;
    while (1) {
        n = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&recvaddr, &recvaddr_len);
        if (n < 0) {
            perror("Ошибка получения ответа");
            close(sockfd);
            exit(EXIT_FAILURE);
        }

        // Пропуск заголовков IP и UDP
        struct iphdr *recv_iph = (struct iphdr *)buffer;
        struct udphdr *recv_udph = (struct udphdr *)(buffer + sizeof(struct iphdr));
        char *recv_data = buffer + sizeof(struct iphdr) + sizeof(struct udphdr);

        // Вывод заголовков IP и UDP после получения
        print_ip_header(recv_iph);
        print_udp_header(recv_udph);

        // Вывод информации об отправителе
        printf("Пакет получен от %s:%d\n", inet_ntoa(recvaddr.sin_addr), ntohs(recvaddr.sin_port));

        // Проверка порта назначения и IP-адреса
        if (ntohs(recv_udph->dest) == 12345 && recv_iph->saddr == iph->daddr && recv_iph->daddr == iph->saddr) {
            recv_data[n - sizeof(struct iphdr) - sizeof(struct udphdr)] = '\0';
            printf("Ответ от сервера: %s\n", recv_data);

            // Вывод полученных данных
            print_data("Полученные данные", recv_data, strlen(recv_data));

            // Проверка различий между отправленными и полученными данными
            if (strcmp(data, recv_data) != 0) {
                printf("Различия между отправленными и полученными данными:\n");
                printf("Отправленные данные: %s\n", data);
                printf("Полученные данные: %s\n", recv_data);
            } else {
                printf("Данные совпадают.\n");
            }
            break;
        } else {
            printf("Получен пакет, предназначенный не для этого клиента.\n");
        }
    }

    close(sockfd);
    printf("Сокет закрыт.\n");
    return 0;
}