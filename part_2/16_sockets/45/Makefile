
.PHONY: all clean

all: broadcast_server broadcast_client multicast_server multicast_client

broadcast_server: broadcast/server.c
	gcc -o broadcast/server broadcast/server.c

# Сборка Broadcast клиента
broadcast_client: broadcast/client.c
	gcc -o broadcast/client broadcast/client.c

# Сборка Multicast сервера
multicast_server: multicast/server.c
	gcc -o multicast/server multicast/server.c

# Сборка Multicast клиента
multicast_client: multicast/client.c
	gcc -o multicast/client multicast/client.c

# Очистка скомпилированных файлов
clean:
	rm -f broadcast/server broadcast/client multicast/server multicast/client