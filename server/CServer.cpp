#include<iostream>
#include<unistd.h>
#include<sys/types.h>
#include<strings.h>
#include<errno.h>
#include"CServer.h"

CServer::CServer()
{
	std::cout<<"server object created"<<std::endl;

}

eErrorType CServer::OpenSocket()
{
	eErrorType Error = SUCCESS;
	//create socket
	this->sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(this->sockfd < 0)
	{
		Error = SOCKETOPENFAIL;
		std::cout<<"Error Code :"<<errno<<std::endl;
		return Error;
	}
	//clear address structure
	bzero((char *) &this->serv_addr, sizeof(this->serv_addr));
	this->portno = 8080;
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY; 
	serv_addr.sin_port = htons(portno);
	
	return Error;
}

eErrorType CServer::BindServer()
{
	eErrorType Error = SUCCESS;
	int bindRet = bind(sockfd,(struct sockaddr *) &this->serv_addr,sizeof(this->serv_addr)); 
	std::cout<<bindRet<<std::endl;
	if(bindRet < 0)
	{
		Error = BINDFAIL;
		std::cout<<"Error Code :"<<errno<<std::endl;
	}
	return Error;
	
}

eErrorType CServer::ListenServer()
{
	eErrorType Error = SUCCESS;
	int lisret = listen(sockfd,5);
	if(lisret < 0)
	{
		Error = LISTENFAIL;
		std::cout<<"Error Code :"<<errno<<std::endl;
	}
	return Error;
	
}

eErrorType CServer::AcceptServer()
{
	eErrorType Error = SUCCESS;
	this->newsockfd = accept(this->sockfd,(struct sockaddr *)&this->cli_addr,&this->clientlen);
	if(this->newsockfd < 0)
	{
		Error = ACCEPTFAIL;
		std::cout<<"Error Code :"<<errno<<std::endl;
	}
	return Error;
}


eErrorType CServer::SendServer(std::string msg)
{
	eErrorType Error = SUCCESS;
	ssize_t sndRet = send(this->newsockfd,msg.c_str(),msg.length(),0);
	if(sndRet < 0)
	{
		Error = SENDFAIL;
		std::cout<<"Error Code :"<<errno<<std::endl;
	}


	return Error;
}


eErrorType CServer::ReceiveServer()
{
	eErrorType Error = SUCCESS;
	bzero(buffer,256);
	ssize_t rdRet = read(this->newsockfd,buffer,256);
	if(rdRet < 0) 
	{
		Error = RECEIVEFAIL;
		std::cout<<"Error Code :"<<errno<<std::endl;
	}
	return Error;
}

void CServer::CloseSocket()
{
	close(this->sockfd);
}
