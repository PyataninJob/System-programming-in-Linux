#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/ip.h>

#define PORT 8080
#define BUFFER_SIZE 1024

typedef struct udp_header {
    uint16_t src_port;
    uint16_t dst_port;
    uint16_t length;
    uint16_t checksum;
} udp_header;

typedef struct ip_header {
    uint8_t ihl:4, version:4;
    uint8_t tos;
    uint16_t tot_len;
    uint16_t id;
    uint16_t frag_off;
    uint8_t ttl;
    uint8_t protocol;
    uint16_t check;
    uint32_t saddr;
    uint32_t daddr;
} ip_header;

void print_udp_header(udp_header *udph) {
    printf("UDP Header:\n");
    printf("  Source Port: %d\n", ntohs(udph->src_port));
    printf("  Destination Port: %d\n", ntohs(udph->dst_port));
    printf("  Length: %d\n", ntohs(udph->length));
    printf("  Checksum: %d\n", ntohs(udph->checksum));
}

void print_ip_header(ip_header *iph) {
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
    const char *message = "Hello, Server!";
    struct sockaddr_in servaddr, recvaddr;
    socklen_t recvaddr_len = sizeof(recvaddr);

    // Creating RAW socket
    if ((sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_UDP)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
    printf("Socket created.\n");

    // Enabling IP_HDRINCL option
    int one = 1;
    if (setsockopt(sockfd, IPPROTO_IP, IP_HDRINCL, &one, sizeof(one)) < 0) {
        perror("Setting IP_HDRINCL failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Forming the packet
    char packet[BUFFER_SIZE];
    ip_header *iph = (ip_header *)packet;
    udp_header *udph = (udp_header *)(packet + sizeof(ip_header));
    char *data = packet + sizeof(ip_header) + sizeof(udp_header);
    strcpy(data, message);

    // Filling IP header
    *iph = (ip_header){
        .ihl = 5,
        .version = 4,
        .tos = 0,
        .tot_len = htons(sizeof(ip_header) + sizeof(udp_header) + strlen(message)),
        .id = 0,
        .frag_off = 0,
        .ttl = 255,
        .protocol = IPPROTO_UDP,
        .check = 0,
        .saddr = inet_addr("127.0.0.1"),
        .daddr = servaddr.sin_addr.s_addr
    };

    // Filling UDP header
    *udph = (udp_header){
        .src_port = htons(12345),  // Arbitrary client port
        .dst_port = htons(PORT),   // Server port
        .length = htons(sizeof(udp_header) + strlen(message)),
        .checksum = 0              // Checksum not used
    };

    // Printing IP and UDP headers before sending
    print_ip_header(iph);
    print_udp_header(udph);

    // Printing data before sending
    print_data("Sending data", data, strlen(message));

    // Sending packet to server
    if (sendto(sockfd, packet, ntohs(iph->tot_len), 0, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("Packet sending failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    printf("Message sent: %s\n", message);

    // Receiving response from server
    int n;
    while (1) {
        n = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&recvaddr, &recvaddr_len);
        if (n < 0) {
            perror("Receiving response failed");
            close(sockfd);
            exit(EXIT_FAILURE);
        }

        // Skipping IP and UDP headers
        ip_header *recv_iph = (ip_header *)buffer;
        udp_header *recv_udph = (udp_header *)(buffer + sizeof(ip_header));
        char *recv_data = buffer + sizeof(ip_header) + sizeof(udp_header);

        // Printing IP and UDP headers after receiving
        print_ip_header(recv_iph);
        print_udp_header(recv_udph);

        // Printing sender information
        printf("Packet received from %s:%d\n", inet_ntoa(recvaddr.sin_addr), ntohs(recvaddr.sin_port));

        // Checking destination port and IP address
        if (ntohs(recv_udph->dst_port) == 12345 && recv_iph->saddr == iph->daddr && recv_iph->daddr == iph->saddr) {
            recv_data[n - sizeof(ip_header) - sizeof(udp_header)] = '\0';
            printf("Response from server: %s\n", recv_data);

            // Printing received data
            print_data("Received data", recv_data, strlen(recv_data));

            // Checking differences between sent and received data
            if (strcmp(data, recv_data) != 0) {
                printf("Differences between sent and received data:\n");
                printf("Sent data: %s\n", data);
                printf("Received data: %s\n", recv_data);
            } else {
                printf("Data matches.\n");
            }
            break;
        } else {
            printf("Received packet not intended for this client.\n");
        }
    }

    close(sockfd);
    printf("Socket closed.\n");
    return 0;
}