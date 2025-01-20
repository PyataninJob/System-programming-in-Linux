CC = gcc
CFLAGS = -Wall -I. -fPIC
LDFLAGS = -shared

all: main

libcalc.so: add.o sub.o mul.o div.o
	$(CC) $(LDFLAGS) -o libcalc.so add.o sub.o mul.o div.o

add.o: add.c add.h
	$(CC) $(CFLAGS) -c add.c

sub.o: sub.c sub.h
	$(CC) $(CFLAGS) -c sub.c

mul.o: mul.c mul.h
	$(CC) $(CFLAGS) -c mul.c

div.o: div.c div.h
	$(CC) $(CFLAGS) -c div.c

main: main.o libcalc.so
	$(CC) -o main main.o -L. -lcalc

main.o: main.c add.h sub.h mul.h div.h
	$(CC) $(CFLAGS) -c main.c

clean:
	rm -f *.o *.so main