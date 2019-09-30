CC = g++
#CFLAG = -g -Wall
CFLAG = -g 

default: client 

client: CFileReading.o CClient.o  ClientMain.o
	$(CC) $(CFLAG) -o client CFileReading.o CClient.o ClientMain.o

CFileReading.o: CFileReading.cpp CFileReading.h
	$(CC) $(CFLAG) -c CFileReading.cpp


CServer.o: CClient.cpp CClient.h
	$(CC) $(CFLAG) -c CClient.cpp

ServerMain.o: ClientMain.cpp
	$(CC) $(CFLAG) -c ClientMain.cpp

clean:
	$(RM) Client *.o
