# Определение компилятора
CC = gcc
CFLAGS = -Wall -Wextra -std=c11

# Директории
MULTIPLEXING_DIR = multiplexing
PRODUCER_CONSUMER_TCP_DIR = producer_consumer/tcp
PRODUCER_CONSUMER_UDP_DIR = producer_consumer/udp
SIMPLE_PARALLEL_TCP_DIR = simple_parallel/tcp
SIMPLE_PARALLEL_UDP_DIR = simple_parallel/udp
THREAD_POOL_TCP_DIR = thread_pool/tcp
THREAD_POOL_UDP_DIR = thread_pool/udp

# Цели для мультиплексирования
MULTIPLEXING_TARGETS = multiplexing/server multiplexing/clientTSP multiplexing/clientUDP

# Цели для producer_consumer
PRODUCER_CONSUMER_TCP_TARGETS = producer_consumer/tcp/server producer_consumer/tcp/client
PRODUCER_CONSUMER_UDP_TARGETS = producer_consumer/udp/server producer_consumer/udp/client

# Цели для simple_parallel
SIMPLE_PARALLEL_TCP_TARGETS = simple_parallel/tcp/server simple_parallel/tcp/client
SIMPLE_PARALLEL_UDP_TARGETS = simple_parallel/udp/server simple_parallel/udp/client

# Цели для thread_pool
THREAD_POOL_TCP_TARGETS = thread_pool/tcp/server thread_pool/tcp/client
THREAD_POOL_UDP_TARGETS = thread_pool/udp/server thread_pool/udp/client

# Все цели
ALL_TARGETS = $(MULTIPLEXING_TARGETS) \
              $(PRODUCER_CONSUMER_TCP_TARGETS) \
              $(PRODUCER_CONSUMER_UDP_TARGETS) \
              $(SIMPLE_PARALLEL_TCP_TARGETS) \
              $(SIMPLE_PARALLEL_UDP_TARGETS) \
              $(THREAD_POOL_TCP_TARGETS) \
              $(THREAD_POOL_UDP_TARGETS)

# Правило по умолчанию
all: $(ALL_TARGETS)

# Правила для мультиплексирования
multiplexing/server: $(MULTIPLEXING_DIR)/server.c
	$(CC) $(CFLAGS) -o $@ $<

multiplexing/clientTSP: $(MULTIPLEXING_DIR)/clientTSP.c
	$(CC) $(CFLAGS) -o $@ $<

multiplexing/clientUDP: $(MULTIPLEXING_DIR)/clientUDP.c
	$(CC) $(CFLAGS) -o $@ $<

# Правила для producer_consumer/tcp
producer_consumer/tcp/server: $(PRODUCER_CONSUMER_TCP_DIR)/server.c
	$(CC) $(CFLAGS) -o $@ $<

producer_consumer/tcp/client: $(PRODUCER_CONSUMER_TCP_DIR)/client.c
	$(CC) $(CFLAGS) -o $@ $<

# Правила для producer_consumer/udp
producer_consumer/udp/server: $(PRODUCER_CONSUMER_UDP_DIR)/server.c
	$(CC) $(CFLAGS) -o $@ $<

producer_consumer/udp/client: $(PRODUCER_CONSUMER_UDP_DIR)/client.c
	$(CC) $(CFLAGS) -o $@ $<

# Правила для simple_parallel/tcp
simple_parallel/tcp/server: $(SIMPLE_PARALLEL_TCP_DIR)/server.c
	$(CC) $(CFLAGS) -o $@ $<

simple_parallel/tcp/client: $(SIMPLE_PARALLEL_TCP_DIR)/client.c
	$(CC) $(CFLAGS) -o $@ $<

# Правила для simple_parallel/udp
simple_parallel/udp/server: $(SIMPLE_PARALLEL_UDP_DIR)/server.c
	$(CC) $(CFLAGS) -o $@ $<

simple_parallel/udp/client: $(SIMPLE_PARALLEL_UDP_DIR)/client.c
	$(CC) $(CFLAGS) -o $@ $<

# Правила для thread_pool/tcp
thread_pool/tcp/server: $(THREAD_POOL_TCP_DIR)/server.c
	$(CC) $(CFLAGS) -o $@ $<

thread_pool/tcp/client: $(THREAD_POOL_TCP_DIR)/client.c
	$(CC) $(CFLAGS) -o $@ $<

# Правила для thread_pool/udp
thread_pool/udp/server: $(THREAD_POOL_UDP_DIR)/server.c
	$(CC) $(CFLAGS) -o $@ $<

thread_pool/udp/client: $(THREAD_POOL_UDP_DIR)/client.c
	$(CC) $(CFLAGS) -o $@ $<

# Очистка
clean:
	rm -f $(ALL_TARGETS)

.PHONY: all clean