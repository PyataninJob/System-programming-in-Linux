Server:
./server 
UDP Server with producer-consumer pattern listening on port 8080...
Received from 127.0.0.1:43723: hi
Received from 127.0.0.1:37655: clietn 2
Received from 127.0.0.1:56780: c3
Received from 127.0.0.1:52867: c4
Received from 127.0.0.1:52867: asdf
Received from 127.0.0.1:43723: asdf
Received from 127.0.0.1:37655: fawefsadf
Received from 127.0.0.1:56780: asrgasdf
Received from 127.0.0.1:52867: a4gsadf

Client 1:
./client 
Enter message: hi
Server echo: hi
Enter message: asdf
Server echo: asdf
Enter message: 
Client 2:
./client 
Enter message: clietn 2
Server echo: clietn 2
Enter message: fawefsadf
Server echo: fawefsadf
Enter message: 

Client 3:
./client 
Enter message: c3
Server echo: c3
Enter message: asrgasdf
Server echo: asrgasdf
Enter message: 

Client 4:
./client 
Enter message: c4
Server echo: c4
Enter message: asdf
Server echo: asdf
Enter message: a4gsadf
Server echo: a4gsadf
Enter message: 
