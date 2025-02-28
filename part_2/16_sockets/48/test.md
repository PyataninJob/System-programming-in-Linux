sudo ./server
Server is running on port 8080...
Received message from client: Hello, Server!
Modified message: Modified: Hello, Server!
Sent modified message back to client.

sudo ./client
Socket created.
IP Header:
  Version: 4
  Header Length: 5
  Total Length: 42
  Protocol: 17
  Source IP: 127.0.0.1
  Destination IP: 127.0.0.1
UDP Header:
  Source Port: 12345
  Destination Port: 8080
  Length: 22
  Checksum: 0
Sending data: 48 65 6c 6c 6f 2c 20 53 65 72 76 65 72 21 
Message sent: Hello, Server!
IP Header:
  Version: 4
  Header Length: 5
  Total Length: 42
  Protocol: 17
  Source IP: 127.0.0.1
  Destination IP: 127.0.0.1
UDP Header:
  Source Port: 12345
  Destination Port: 8080
  Length: 22
  Checksum: 0
Packet received from 127.0.0.1:0
Received packet not intended for this client.
IP Header:
  Version: 4
  Header Length: 5
  Total Length: 52
  Protocol: 17
  Source IP: 127.0.0.1
  Destination IP: 127.0.0.1
UDP Header:
  Source Port: 8080
  Destination Port: 12345
  Length: 32
  Checksum: 65075
Packet received from 127.0.0.1:0
Response from server: Modified: Hello, Server!
Received data: 4d 6f 64 69 66 69 65 64 3a 20 48 65 6c 6c 6f 2c 20 53 65 72 76 65 72 21 
Differences between sent and received data:
Sent data: Hello, Server!
Received data: Modified: Hello, Server!
Socket closed.