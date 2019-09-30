#include<iostream>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include"CServer.h"
#include"CFileReading.h"

void PrintErrorMSg(std::string errMsg)
{
	std::cout<<errMsg<<std::endl;
}

void ReceiveCommand(CServer * serObj,std::string &receivedcmd)
{
	eErrorType Error = SUCCESS;
	Error = serObj->ReceiveServer();
	if(Error != SUCCESS)
	{
		PrintErrorMSg("Server  Fail");
	}
	std::cout<<"Received msg :"<<serObj->buffer<<std::endl;
	receivedcmd = serObj->buffer;
	Error = serObj->SendServer("Received command : " + receivedcmd);
	if(Error != SUCCESS)
	{
		PrintErrorMSg("Server Send Fail");
	}

	
}

void ReceivedFileName(CServer * serObj,std::string &receivedFileName)
{
	eErrorType Error = SUCCESS;
	Error = serObj->ReceiveServer();
	if(Error != SUCCESS)
	{
		PrintErrorMSg("Server  Fail");
	}
	std::cout<<"Received File Name :"<<serObj->buffer<<std::endl;
	receivedFileName = serObj->buffer;
	Error = serObj->SendServer("Received File : " + receivedFileName);
	if(Error != SUCCESS)
	{
		PrintErrorMSg("Server Send Fail");
	}
}

void RenameFile(CServer * serObj,std::string oldFileName)
{
	eErrorType Error = SUCCESS;
	std::string NewFileName;
	CFileReading FileObj;
	//receiving old file name
	ReceivedFileName(serObj,NewFileName);
	Error = FileObj.RenameFile(oldFileName,NewFileName);
	if(Error == SUCCESS)
	{
		Error = serObj->SendServer("Rename successfull : " );
		std::cout<<"Rename successfull"<<std::endl;
	}
	else
	{
		Error = serObj->SendServer("Rename Fail : " );
		std::cout<<"Rename Fail"<<std::endl;
	}
}

void RemoveFile(CServer * serObj,std::string FileName)
{
	eErrorType Error = SUCCESS;
	CFileReading FileObj;
	Error = FileObj.RemoveFile(FileName);
	if(Error == SUCCESS)
	{
		Error = serObj->SendServer("Remove successfull : " );
		std::cout<<"Remove successfull"<<std::endl;
	}
	else
	{
		Error = serObj->SendServer("Remove Fail : " );
		std::cout<<"Remove Fail"<<std::endl;
	}
}

void UploadFile(CServer * serObj,std::string FileName)
{
        eErrorType Error = SUCCESS;
        CFileReading FileObj;
	std::string ReceivedFileData;
        Error = FileObj.OpenUpdown(FileName);
        while(Error == SUCCESS)
        {
		Error = serObj->ReceiveServer();
		if(Error != SUCCESS)
			break;
                if( strcmp(serObj->buffer,"closefile") != 0)
                {
                        bzero(FileObj.buff,256);
                        bcopy(serObj->buffer,FileObj.buff,255);
                        Error = FileObj.WriteFile(255);
			std::cout<<"Data writen to file :"<<FileObj.buff<<std::endl;
                }
		std::cout<<"Received Filedata :"<<serObj->buffer<<std::endl;
		ReceivedFileData = serObj->buffer;
		Error = serObj->SendServer("Received Filedata : " + ReceivedFileData);
		if(Error != SUCCESS)
		{
			PrintErrorMSg("Server Send Fail");
			break;
		}
                if(strcmp(serObj->buffer,"closefile") == 0)
                {
			std::cout<<"Received closefile command :"<<serObj->buffer<<std::endl;
			Error = FileObj.CloseFile();
			break;
                }
        }
        Error = FileObj.CloseFile();
}

void DownloadFile(CServer * serObj,std::string FileName)
{
	eErrorType Error = SUCCESS;
        CFileReading FileObj;
        int readsize=0;
	std::string SendFileData;
        Error = FileObj.OpenFile(FileName);
	if(Error != SUCCESS)
        {
		SendFileData = "closefile";
                std::cout<<"Send close command :"<<SendFileData<<std::endl;
		Error = serObj->SendServer(SendFileData);
                return;
        }
	while( Error == SUCCESS)
	{
		readsize =0;
		Error = FileObj.ReadFile(&readsize);
		std::cout<<"Read "<<readsize<<" from file"<<std::endl;
		if( Error != ENDOFFILE)
		{
			SendFileData = FileObj.buff;
			std::cout<<"send data from server : "<<SendFileData<<std::endl;
			Error = serObj->SendServer(SendFileData);
		
			//acknomledge write data
			Error = serObj->ReceiveServer();
			if(Error != SUCCESS)
			{
				std::cout<<"acknowledge from client failed for write file data"<<std::endl;
                        	break;
			}
			std::cout<<"Data written on client : " <<serObj->buffer<<std::endl;
		}
		else if(Error == ENDOFFILE)
        	{
			SendFileData = "closefile";
                	std::cout<<"Send close command :"<<SendFileData<<std::endl;
                	Error = serObj->SendServer(SendFileData);
			//acknomledge write data
                	Error = serObj->ReceiveServer();
                	if(Error != SUCCESS)
                	{
                        	std::cout<<"acknowledge from client failed for write file data"<<std::endl;
                	}
                        break;
		}
	}
	Error = FileObj.CloseFile();	
}

int main()
{
	CServer serObj;
	eErrorType Error = SUCCESS;
	std::string receivedcmd;
	std::string receivedFileName;
	Error = serObj.OpenSocket();
	if(Error != SUCCESS)
		std::cout<<"socket open fail"<<std::endl;
	std::cout<<Error<<std::endl;
	Error = serObj.BindServer();
	if(Error != SUCCESS)
	{
		PrintErrorMSg("Server Bind Fail");
		return 1;
	}

	Error = serObj.ListenServer();
	if(Error != SUCCESS)
	{
		PrintErrorMSg("Server Listen Fail");
		return 1;
	}
	while(true)
	{

		Error = serObj.AcceptServer();
		if(Error == SUCCESS)
		{
			std::cout<<"Server got connection from "<<inet_ntoa(serObj.cli_addr.sin_addr)<<" port "<<ntohs(serObj.cli_addr.sin_port)<<std::endl;
		}
		ReceiveCommand(&serObj,receivedcmd);
		std::cout<<"Received command :"<<receivedcmd<<std::endl;

		ReceivedFileName(&serObj,receivedFileName);
		std::cout<<"Received File :"<<receivedFileName<<std::endl;
		
		if(receivedcmd.compare("Rename") == 0)
		{
			RenameFile(&serObj,receivedFileName);
		}
		else if(receivedcmd.compare("delete") == 0)
		{
			RemoveFile(&serObj,receivedFileName);
		}
		else if(receivedcmd.compare("upload") == 0)
		{
			UploadFile(&serObj,receivedFileName);
		}
		else if(receivedcmd.compare("download") == 0)
		{
			DownloadFile(&serObj,receivedFileName);
		}
	}

	serObj.CloseSocket();
	return 0;
}
