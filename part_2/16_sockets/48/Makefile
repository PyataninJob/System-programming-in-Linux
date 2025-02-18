CC = gcc
CFLAGS = -Wall -Wextra
TARGETS = server client
SRC_SERVER = server.c
SRC_CLIENT = client.c

all: $(TARGETS)

server: $(SRC_SERVER)
	$(CC) $(CFLAGS) -o server $(SRC_SERVER)

client: $(SRC_CLIENT)
	$(CC) $(CFLAGS) -o client $(SRC_CLIENT)

clean:
	rm -f $(TARGETS)

.PHONY: all clean