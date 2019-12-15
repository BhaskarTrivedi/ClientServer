With high computational power and distributed system, inter process communication has become very critical to success of your complete model design. Often student get confuse with basic communication procedure. In this example I created IPC between client and server and performed the file upload, download or delete operation on server. I created wrapper to create client, server and file handling so it will be easy for student to modified it for their own purpose.
Server wrapper
Server is responsible to create a server and perform the server related task. server.h and server.cpp create the server wrapper. 
Server Operation
  1) Opensocket : Perform the open socket operation
  2) BindSocket : Bind server using bind API call
  3) ListenSenver : Initiate Listen
  4) Acceptserver : Accept any incoming request
  5) SendServer :  Send message from server to client
  6) ReceiveServer : Receive message from client
  
  
