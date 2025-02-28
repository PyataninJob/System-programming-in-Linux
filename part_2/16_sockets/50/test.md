### Использование Wireshark

#### Пинг хост-машины в виртуальной сети

```bash
ping 10.241.88.25
```

Вывод:
```
PING 10.241.88.25 (10.241.88.25) 56(84) bytes of data.
```

#### Применение фильтра: `ip.addr == 10.241.88.25`

Наблюдаемые пакеты:

| No. | Time           | Source IP         | Destination IP    | Protocol | Length | Info                                                                 |
|-----|----------------|-------------------|-------------------|----------|--------|----------------------------------------------------------------------|
| 11873 | 614.149486893 | 10.241.66.204     | 10.241.88.25      | ICMP     | 98     | Echo (ping) request id=0x212d, seq=1/256, ttl=64 (no response found!) |
| 11896 | 615.211223571 | 10.241.66.204     | 10.241.88.25      | ICMP     | 98     | Echo (ping) request id=0x212d, seq=2/512, ttl=64 (no response found!) |
| 11912 | 616.232090463 | 10.241.66.204     | 10.241.88.25      | ICMP     | 98     | Echo (ping) request id=0x212d, seq=3/768, ttl=64 (no response found!) |
| 11932 | 617.254611830 | 10.241.66.204     | 10.241.88.25      | ICMP     | 98     | Echo (ping) request id=0x212d, seq=4/1024, ttl=64 (no response found!) |
| 11952 | 618.277959797 | 10.241.66.204     | 10.241.88.25      | ICMP     | 98     | Echo (ping) request id=0x212d, seq=5/1280, ttl=64 (no response found!) |
| 11964 | 619.302052391 | 10.241.66.204     | 10.241.88.25      | ICMP     | 98     | Echo (ping) request id=0x212d, seq=6/1536, ttl=64 (no response found!) |

#### Разработка программы

Сначала я столкнулся с ошибкой:

```
75  3.256201999  105.115.32.97  32.116.101.115  IPv4  44  [Malformed Packet]
```

Я думал, что это может быть проблема с VPN или что-то, что я делал не так. Я попробовал принять свой пакет:

```
Получен пакет длиной 60 байт
Получен пакет длиной 60 байт
Получен пакет длиной 44 байт
Пакет от источника 08:00:27:d2:82:46 к назначению 00:1A:2B:3C:4D:5E с типом 0x88B5
Данные: Hello, this is a test message!
Получен пакет длиной 60 байт
Получен пакет длиной 86 байт
Получен пакет длиной 60 байт
Получен пакет длиной 60 байт
```

Теперь ошибки нет. Странно.

```
110  4.956934160  PCSSystemtec_d2:82:46  AyecomTechno_3c:4d:5e  0x88b5  44  Local Experimental Ethertype 1
```

#### Новое: Заполнение 3 заголовков

Я придумал себе задачу. Я не использовал C на Windows и подумал, почему бы не использовать WSL? Потребовалось много времени, чтобы все связать. Оказалось, что адаптер нужно настраивать отдельно на WSL, а затем на Windows. Я создал две виртуальные машины в VirtualBox.

- Клиент: 192.168.56.10
- Сервер: 192.168.56.11

Настраиваем адаптер на внутренний адаптер хоста. На каждой машине появляется интерфейс `enp0s8`. Настраиваем его на каждой машине:

```bash
ip a
```

MAC-адреса:

```bash
ip link show
```

или

```bash
cat /sys/class/net/enp0s8/address
```

Добавляем IP-адреса:

```bash
sudo ip addr add 192.168.56.10/24 dev enp0s8
sudo ip addr add 192.168.56.11/24 dev enp0s8
```

Активируем интерфейсы:

```bash
sudo ip link set enp0s8 up
```

Пингуем и видим, что пинг идет.

#### Вывод сервера

```bash
sudo ./server
Packet from unknown source (100.241.10.241) ignored.
Received packet from:
  Source MAC: 08:00:27:59:07:f4
  Destination MAC: 08:00:27:bf:99:aa
  Source IP: 192.168.56.10
Data: Hello, Server!
Modified Data: Modified: Hello, Server!
Sending packet with length: 60
Sending packet with length: 66
Response sent successfully.
Packet from unknown source (100.241.10.241) ignored.
Packet from unknown source (100.241.10.241) ignored.
```

#### Вывод клиента

```bash
sudo ./client
Packet sent successfully.
Received packet with length: 66
Full packet (66 bytes):
08 00 27 59 07 f4 08 00 27 bf 99 aa 08 00 45 00
00 34 00 00 00 00 40 11 00 00 c0 a8 38 0a c0 a8
38 0b 30 39 1f 90 00 20 00 00 4d 6f 64 69 66 69
65 64 3a 20 48 65 6c 6c 6f 2c 20 53 65 72 76 65
72 21
Received response (24 bytes): Modified: Hello, Server!
```
