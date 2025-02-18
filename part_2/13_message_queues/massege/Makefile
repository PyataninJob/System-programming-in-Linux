CC=gcc
CFLAGS=-Wall -Wextra

.PHONY: all clean

all: posix-server posix-client sysv-server sysv-client

posix-server: posix/server.c
	$(CC) $(CFLAGS) -o posix/server posix/server.c

posix-client: posix/client.c
	$(CC) $(CFLAGS) -o posix/client posix/client.c

sysv-server: sysv/server.c
	$(CC) $(CFLAGS) -o sysv/server sysv/server.c

sysv-client: sysv/client.c
	$(CC) $(CFLAGS) -o sysv/client sysv/client.c

clean:
	rm -f posix/server posix/client sysv/server sysv/client