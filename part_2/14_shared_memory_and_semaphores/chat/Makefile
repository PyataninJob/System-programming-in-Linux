CC = gcc
CFLAGS = -Wall -pthread
LDFLAGS = -lncurses

all: server client

server: server.c shared_memory.h
	$(CC) $(CFLAGS) -o server server.c

client: client.c shared_memory.h
	$(CC) $(CFLAGS) -o client client.c $(LDFLAGS)

clean:
	rm -f server client