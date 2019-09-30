#ifndef CCLIENT_H
#define CCLIENT_H
#include<netinet/in.h>
#include<netdb.h>
#include<string>
#include"Utility.h"

class CClient
{
public:
	int sockfd,portno;
	struct sockaddr_in serv_addr;
	struct hostent *server;
	char buffer[256];
	CClient();
	eErrorType OpenSocket();
	eErrorType GetHost(std::string hostname);
	eErrorType ClientConnect();
	eErrorType ClientWrite();
	eErrorType ClientRead();
	void CloseSocket();
	
};

#endif
