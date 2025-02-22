Почемуто пока я пытался это сделать то постоянно получал какойто левый пакет неизвестно откуда и потом клиент прекращал работу. Но с добавлением фильтрации я хотябы начал получать ожидаемый пакет.

./server
Сокет создан.
Сокет привязан к порту 8080.
Получено сообщение от клиента: Привет, сервер
Модифицированное сообщение: Привет,_сервер
Модифицированное сообщение отправлено клиенту.


./client 
sudo ./client 
Сокет создан.
IP Header:
  Version: 4
  Header Length: 5
  Total Length: 54
  Protocol: 17
  Source IP: 127.0.0.1
  Destination IP: 127.0.0.1
UDP Header:
  Source Port: 12345
  Destination Port: 8080
  Length: 34
  Checksum: 0
Отправляемые данные: d0 9f d1 80 d0 b8 d0 b2 d0 b5 d1 82 2c 20 d1 81 d0 b5 d1 80 d0 b2 d0 b5 d1 80 
Сообщение отправлено: Привет, сервер
IP Header:
  Version: 4
  Header Length: 5
  Total Length: 54
  Protocol: 17
  Source IP: 127.0.0.1
  Destination IP: 127.0.0.1
UDP Header:
  Source Port: 12345
  Destination Port: 8080
  Length: 34
  Checksum: 0
Пакет получен от 127.0.0.1:0
Получен пакет, предназначенный не для этого клиента.
IP Header:
  Version: 4
  Header Length: 5
  Total Length: 54
  Protocol: 17
  Source IP: 127.0.0.1
  Destination IP: 127.0.0.1
UDP Header:
  Source Port: 8080
  Destination Port: 12345
  Length: 34
  Checksum: 65077
Пакет получен от 127.0.0.1:0
Ответ от сервера: Привет,_сервер
Полученные данные: d0 9f d1 80 d0 b8 d0 b2 d0 b5 d1 82 2c 5f d1 81 d0 b5 d1 80 d0 b2 d0 b5 d1 80 
Различия между отправленными и полученными данными:
Отправленные данные: Привет, сервер
Полученные данные: Привет,_сервер
Сокет закрыт.
