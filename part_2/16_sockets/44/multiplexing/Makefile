CC = gcc
CFLAGS = -Wall -Wextra

all: server tcp_client udp_client

server: server.c
	$(CC) $(CFLAGS) -o server server.c

tcp_client: clientTCP.c
	$(CC) $(CFLAGS) -o clientTCP clientTCP.c

udp_client: clientUDP.c
	$(CC) $(CFLAGS) -o clientUDP clientUDP.c

clean:
	rm -f server clientUDP clientTCP

.PHONY: all clean