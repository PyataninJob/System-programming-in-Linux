CC = gcc
CFLAGS = -Wall -Wextra
TARGETS = server client #server_ client_
SRC_SERVER = server.c
SRC_CLIENT = client.c
# SRC_SERVER_ = server_.c
# SRC_CLIENT_ = client_.c

all: $(TARGETS)

server: $(SRC_SERVER)
	$(CC) $(CFLAGS) -o server $(SRC_SERVER)
# $(CC) $(CFLAGS) -o server_ $(SRC_SERVER)_

client: $(SRC_CLIENT)
	$(CC) $(CFLAGS) -o client $(SRC_CLIENT)
# $(CC) $(CFLAGS) -o client_ $(SRC_CLIENT_)

clean:
	rm -f $(TARGETS)

.PHONY: all clean