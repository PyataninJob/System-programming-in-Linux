Пока весь пулл занят 6 будет в ожидании пока кто-то не отключится
Server:
./server 
TCP Server with thread pool listening on port 8080...
New client connected: 127.0.0.1:55344
Received: client 1
New client connected: 127.0.0.1:40956
Received: client 2
New client connected: 127.0.0.1:40180
Received: client 3
New client connected: 127.0.0.1:44676
Received: client 4
New client connected: 127.0.0.1:54636
Received: client5 
New client connected: 127.0.0.1:53068
Received: hi
Received: client 6 can't
Received: client 6



Client 1:
 ./client 
Connected to server at 127.0.0.1:8080
Enter message: client 1
Server echo: client 1
Enter message: hi
Server echo: hi
Enter message: client 6 can't
Server echo: client 6 can't
Enter message: 

Client 2:
./client 
Connected to server at 127.0.0.1:8080
Enter message: client 2
Server echo: client 2
Enter message: 

Client 3:
./client 
Connected to server at 127.0.0.1:8080
Enter message: client 3
Server echo: client 3
Enter message: 

Client 4:
./client client4
Connected to server at 127.0.0.1:8080
Enter message: client 4
Server echo: client 4
Enter message: 

Client 5:
./client 
Connected to server at 127.0.0.1:8080
Enter message: client5 
Server echo: client5 
Enter message: 

Client 6:
./client 
Connected to server at 127.0.0.1:8080
Enter message: client 6

