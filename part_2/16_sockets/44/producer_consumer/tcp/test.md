Пока я не освободиол место 3 клиента то четвертый не стал обробатываться. 4 Слиент накапливает задачи.
Server:
./server 
TCP Server with producer-consumer pattern listening on port 8080...
New client connected: 127.0.0.1:47006
Received: hi
New client connected: 127.0.0.1:40066
Received: adf
New client connected: 127.0.0.1:56314
Received: asdaef
New client connected: 127.0.0.1:56324
Received: asf
Received: wef
Received: asdf

Client 1:
./client 
Connected to server at 127.0.0.1:8080
Enter message: hi
Server echo: hi
Enter message: 


Client 2:
./client 
Connected to server at 127.0.0.1:8080
Enter message: adf
Server echo: adf
Enter message: 

Client 3:
./client 
Connected to server at 127.0.0.1:8080
Enter message: asdaef
Server echo: asdaef
Enter message: ^C

Client 4:
./client 
Connected to server at 127.0.0.1:8080
Enter message: asf
wef
asdf
Server echo: asf
Enter message: Server echo: wef
Enter message: Server echo: asdf
Enter message: 
