Вот UDP парлельный имеет смысл вроде только если есть какието затратные опирации?
Server:
./server 
UDP Server listening on port 8080...
Received from 127.0.0.1:54151: client 1
Received from 127.0.0.1:35370: client 2
Received from 127.0.0.1:35370: hi client 1)))
Received from 127.0.0.1:54151: i can't see your masseges...


Client 1:
./client 
Enter message: client 1
Server echo: client 1
Enter message: i can't see your masseges...
Server echo: i can't see your masseges...
Enter message: 


Client2:
./client 
Enter message: client 2
Server echo: client 2
Enter message: hi client 1)))
Server echo: hi client 1)))
Enter message: 