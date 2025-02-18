CC = gcc
CFLAGS = -Wall -Wextra
TARGETS = signal_wait signal_send block_sigint event_loop

all: $(TARGETS)

signal_wait: signal_wait.c
	$(CC) $(CFLAGS) -o signal_wait signal_wait.c

signal_sen: signal_sen.c
	$(CC) $(CFLAGS) -o signal_send signal_send.c

block_sigint: block_sigint.c
	$(CC) $(CFLAGS) -o block_sigint block_sigint.c

event_loop: event_loop.c
	$(CC) $(CFLAGS) -o event_loop event_loop.c

clean:
	rm -f $(TARGETS)