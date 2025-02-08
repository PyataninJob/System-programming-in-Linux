# Компилятор и флаги
CC = gcc
CFLAGS = -Wall -Wextra

# Директории
AF_INET_TCP_DIR = AF_INET_TCP
AF_INET_UDP_DIR = AF_INET_UDP
LOC_TCP_DIR = loc_TCP
LOC_UDP_DIR = loc_UDP

# Цели для всех программ
all: af_inet_tcp af_inet_udp loc_tcp loc_udp

# TCP через интернет (AF_INET)
af_inet_tcp:
	$(CC) $(CFLAGS) -o $(AF_INET_TCP_DIR)/client $(AF_INET_TCP_DIR)/client.c
	$(CC) $(CFLAGS) -o $(AF_INET_TCP_DIR)/server $(AF_INET_TCP_DIR)/server.c

# UDP через интернет (AF_INET)
af_inet_udp:
	$(CC) $(CFLAGS) -o $(AF_INET_UDP_DIR)/client $(AF_INET_UDP_DIR)/client.c
	$(CC) $(CFLAGS) -o $(AF_INET_UDP_DIR)/server $(AF_INET_UDP_DIR)/server.c

# Локальный TCP (AF_LOCAL)
loc_tcp:
	$(CC) $(CFLAGS) -o $(LOC_TCP_DIR)/client $(LOC_TCP_DIR)/client.c
	$(CC) $(CFLAGS) -o $(LOC_TCP_DIR)/server $(LOC_TCP_DIR)/server.c

# Локальный UDP (AF_LOCAL)
loc_udp:
	$(CC) $(CFLAGS) -o $(LOC_UDP_DIR)/client $(LOC_UDP_DIR)/client.c
	$(CC) $(CFLAGS) -o $(LOC_UDP_DIR)/server $(LOC_UDP_DIR)/server.c

# Очистка скомпилированных файлов
clean:
	rm -f $(AF_INET_TCP_DIR)/client $(AF_INET_TCP_DIR)/server
	rm -f $(AF_INET_UDP_DIR)/client $(AF_INET_UDP_DIR)/server
	rm -f $(LOC_TCP_DIR)/client $(LOC_TCP_DIR)/server
	rm -f $(LOC_UDP_DIR)/client $(LOC_UDP_DIR)/server

# Псевдоцель для предотвращения конфликтов с файлами
.PHONY: all af_inet_tcp af_inet_udp loc_tcp loc_udp clean