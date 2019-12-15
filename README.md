With high computational power and distributed system, inter process communication has become very critical to success of your complete model design. Often student get confuse with basic communication procedure. 
In this example I created IPC between client and server and performed the file upload, download,rename or delete operation on server. I created wrapper to create client, server and file handling so it will be easy for student to modified it for their own purpose.

Server wrapper
<br/>Server is responsible to create a server and perform the server related task. server.h and server.cpp create the server wrapper. 
Server Operation
  1) Opensocket : Perform the open socket operation
  2) BindSocket : Bind server using bind API call
  3) ListenSenver : Initiate Listen
  4) Acceptserver : Accept any incoming request
  5) SendServer :  Send message from server to client
  6) ReceiveServer : Receive message from client
  7) CloseSocket : close client at end of communication
  
 <br/> Client Wrapper: 
   <br/> 1) OpenSocket : Open socket
   <br/> 2) GetHost : Get host to connect with
  <br/>  3) ClientConnect : Connect client to server
  <br/>  4) ClientWrite :  Write data for server
  <br/>  5) ClientRead : Read data from server
  <br/>  6) CloseSocket :  close client at end of communication
 <br/>   
 <br/> File handling wrapper
<br/>    1) OpenFile : Open file for basic operation
<br/>    2) CloseFile :  close file after successful operation
<br/>    3) ReadFile : Read file data
<br/>    4) WriteFile : Write data to file
<br/>    5) RemoveFile : Delete file
<br/>    6) RenameFile : Create and rename old file to new file

<br/>To runcode 
<br/>1) download file to ubuntu machine
<br/>2) run make file from server abd client directory
<br/>  execute make
<br/>	make
<br/>	g++ -g  -c CFileReading.cpp
<br/>	g++ -g  -c CServer.cpp
<br/>	g++ -g  -c ServerMain.cpp
<br/>	g++ -g  -o server CFileReading.o CServer.o ServerMain.o

<br/>3) run ./server and ./client


    
