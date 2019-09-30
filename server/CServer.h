#ifndef CSERVER_H
#define CSERVER_H
#include<netinet/in.h>
#include<sys/socket.h>
#include"Utility.h"

class CServer
{
public:
	int sockfd,newsockfd,portno;
	socklen_t clientlen;
	char buffer[256];
	struct sockaddr_in serv_addr, cli_addr;

	CServer();
	eErrorType OpenSocket();
	eErrorType BindServer();
	eErrorType ListenServer();
	eErrorType AcceptServer();
	eErrorType SendServer(std::string msg);
	eErrorType ReceiveServer(); 
	void CloseSocket();
	
	
};


#endif
