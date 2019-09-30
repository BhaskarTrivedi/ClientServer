#include<unistd.h>
#include<iostream>
#include<strings.h>
#include<stdio.h>
#include<string.h>
#include <errno.h>
#include"CClient.h"

CClient::CClient()
{
	this->portno = 8080;
	this->server = NULL;
}

eErrorType CClient::OpenSocket()
{
	eErrorType Error = SUCCESS;
	this->sockfd = socket(AF_INET,SOCK_STREAM,0);
	if(this->sockfd < 0)
	{
		Error = SOCKETOPENFAIL;
		std::cout<<"Error Code :"<<errno<<std::endl;
	}
	return Error;
}


eErrorType CClient::GetHost(std::string hostname)
{
	eErrorType Error = SUCCESS;
	this->server = gethostbyname(hostname.c_str());
	if(this->server == NULL)
	{
		Error = GETHOSTFAIL;
		std::cout<<"Error Code :"<<errno<<std::endl;
	}
	return Error;
}

eErrorType CClient::ClientConnect()
{
	eErrorType Error = SUCCESS;
	bzero((char*)&this->serv_addr,sizeof(this->serv_addr));
	this->serv_addr.sin_family = AF_INET;
	bcopy((char *)this->server->h_addr,(char *)&this->serv_addr.sin_addr.s_addr,this->server->h_length);
	this->serv_addr.sin_port = htons(portno);
	int conRet = connect(this->sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr));
	if(conRet < 0)
	{
		Error = CONNECTFAIL;
		std::cout<<"Error Code :"<<errno<<std::endl;
	}
	return Error;
}

eErrorType CClient::ClientWrite()
{
	eErrorType Error = SUCCESS;

	int WrtRet = write(this->sockfd,this->buffer,strlen(this->buffer));
	if(WrtRet < 0)
	{
		Error = WRITEFAIL;
		std::cout<<"Error Code :"<<errno<<std::endl;
	}
	
	return Error;
}

eErrorType CClient::ClientRead()
{
	eErrorType Error = SUCCESS;
	bzero(this->buffer,256);
	int RdRet = read(this->sockfd,this->buffer,255);
	if(RdRet < 0)
	{
		Error = READFAIL;
		std::cout<<"Error Code :"<<errno<<std::endl;
	}
	return Error;
}

void CClient::CloseSocket()
{
	close(this->sockfd);
}
