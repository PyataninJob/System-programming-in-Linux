./server 
Ожидание подключения клиента...
Сообщение от клиента: 
Ответ отправлен

nc -zv 192.168.0.105 8080
Connection to 192.168.0.105 8080 port [tcp/http-alt] succeeded!

______________________________________


(base) pyatanin@pyatanin-VirtualBox:~/VSproj/C/part_2/16_sockets/42/AF_INET_TCP$ ./server 
Ожидание подключения клиента...
Сообщение от клиента: Привет
Ответ отправлен
(base) pyatanin@pyatanin-VirtualBox:~/VSproj/C/part_2/16_sockets/42/AF_INET_TCP$ 



pyatanin@Pyatanin:/mnt/d/VSProject/c$ gcc client.c -o client
pyatanin@Pyatanin:/mnt/d/VSProject/c$ ./client
Сообщение отправлено
Ответ от сервера: Здарова
pyatanin@Pyatanin:/mnt/d/VSProject/c$