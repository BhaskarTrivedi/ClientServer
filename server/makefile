CC = g++
#CFLAG = -g -Wall
CFLAG = -g 

default: server

server: CFileReading.o CServer.o  ServerMain.o
	$(CC) $(CFLAG) -o server CFileReading.o CServer.o ServerMain.o

CFileReading.o: CFileReading.cpp CFileReading.h
	$(CC) $(CFLAG) -c CFileReading.cpp


CServer.o: CServer.cpp CServer.h
	$(CC) $(CFLAG) -c CServer.cpp

ServerMain.o: ServerMain.cpp
	$(CC) $(CFLAG) -c ServerMain.cpp

clean:
	$(RM) server *.o
