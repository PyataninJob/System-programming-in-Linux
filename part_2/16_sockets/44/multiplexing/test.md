Server:
./server
Server is running...
New TCP client connected. Socket: 5
New TCP client connected. Socket: 6
Client 1:
./
clientTSP ./clientTCP
Connected to TCP server. Waiting for time updates...
Server Time: TCP Server Time: Sat Feb 22 14:33:33 2025
Server Time: TCP Server Time: Sat Feb 22 14:33:44 2025
Server Time: TCP Server Time: Sat Feb 22 14:33:49 2025
Client 2:
./clientUDP
UDP client started. Sending requests to server...
Server Time: UDP Server Time: Sat Feb 22 14:33:44 2025
Server Time: UDP Server Time: Sat Feb 22 14:33:49 2025
Server Time: UDP Server Time: Sat Feb 22 14:33:59 2025
Client 3:
./clientTCP
Connected to TCP server. Waiting for time updates...
Server Time: TCP Server Time: Sat Feb 22 14:33:49 2025
Server Time: TCP Server Time: Sat Feb 22 14:33:54 2025
Server Time: TCP Server Time: Sat Feb 22 14:33:59 2025

Client 4:
./clientUDP
UDP client started. Sending requests to server...
Server Time: UDP Server Time: Sat Feb 22 14:33:54 2025
Server Time: UDP Server Time: Sat Feb 22 14:34:04 2025
Server Time: UDP Server Time: Sat Feb 22 14:34:14 2025
Server Time: UDP Server Time: Sat Feb 22 14:34:24 2025