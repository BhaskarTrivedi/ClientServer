#include<iostream>
#include<unistd.h>
#include<string.h>
#include<strings.h>
#include"CClient.h"
#include"CFileReading.h"

void PrintError(std::string errMsg)
{
	std::cout<<errMsg<<std::endl;
}

int menu()
{
	int choice=0;
	std::cout<<"Please select the required operation "<<std::endl;
	std::cout<<"1) UPLOAD"<<std::endl;
	std::cout<<"2) DOWNLOAD"<<std::endl;
	std::cout<<"3) DELETE"<<std::endl;
	std::cout<<"4) RENAME"<<std::endl;
	std::cin>>choice;
	return choice; 
}

std::string CreateCommand(int choice)
{
	std::string cmd;
	switch(choice)	
	{
		case 1:
			cmd = "upload";
			break;
		case 2:
			cmd = "download";
			break;
		case 3:
			cmd = "delete";
			break;
		default:
			cmd = "Rename";
			
	}
	return cmd;
}

void SendCommand(CClient *clientObj,std::string cmd)
{
	eErrorType Error = SUCCESS;
	
	//send command
	bzero(clientObj->buffer,256);
	strcpy(clientObj->buffer,cmd.c_str());
	Error = clientObj->ClientWrite();
	if(Error != SUCCESS)
	{
		PrintError("Unable to Write to server from client");
	}

	//acknowlegde received command
	Error = clientObj->ClientRead();
	if(Error != SUCCESS)
	{
		PrintError("Client Unable to read from server");
	}
	std::cout<<"Message Received From server :"<<clientObj->buffer<<std::endl;
}

void SendFilename(CClient *clientObj,std::string Filename)
{
	eErrorType Error = SUCCESS;
	
	//send command
	bzero(clientObj->buffer,256);
	strcpy(clientObj->buffer,Filename.c_str());
	Error = clientObj->ClientWrite();
	if(Error != SUCCESS)
	{
		PrintError("Unable to Write to server from client");
	}

	//acknowlegde received command
	Error = clientObj->ClientRead();
	if(Error != SUCCESS)
	{
		PrintError("Client Unable to read from server");
	}
	std::cout<<"Message Received From server :"<<clientObj->buffer<<std::endl;
}

void SendRename(CClient *clientObj)
{
	eErrorType Error = SUCCESS;
	std::cout<<"Enter New File Name : ";
	std::string NewFilename;
	std::cin>>NewFilename;
	SendFilename(clientObj,NewFilename);
	
	//acknowlegde received command
	Error = clientObj->ClientRead();
	if(Error != SUCCESS)
	{
		PrintError("Client Unable to read from server");
	}
	std::cout<<"Message Received From server :"<<clientObj->buffer<<std::endl;
}

void UploadFile(CClient *clientObj,std::string FileName)
{
	eErrorType Error = SUCCESS;
	CFileReading FileObj;
	int readsize=0;
	Error = FileObj.OpenFile(FileName);
	if(Error != SUCCESS)
	{
		bzero(clientObj->buffer,256);
		strcpy(clientObj->buffer,"closefile");
		std::cout<<"Send close command :"<<clientObj->buffer<<std::endl;
		Error = clientObj->ClientWrite();
		return;
	}
	while(Error == SUCCESS)
	{
		readsize =0;
		Error = FileObj.ReadFile(&readsize);
		std::cout<<"Read "<<readsize<<" from file"<<std::endl;	
		if( Error != ENDOFFILE)
		{
			bzero(clientObj->buffer,256);
			//strcpy(clientObj->buffer,"closefile");
			bcopy(FileObj.buff,clientObj->buffer,readsize);
			std::cout<<"Data sent from client : "<<clientObj->buffer<<std::endl;
			Error = clientObj->ClientWrite();
			
			//acknowlegde received command
			Error = clientObj->ClientRead();
			std::cout<<"Data written on server :"<<clientObj->buffer<<std::endl;
			if(Error != SUCCESS)
			{
				PrintError("Client Unable to read from server");
				break;
			}
		}
		else if(Error == ENDOFFILE)
		{
			bzero(clientObj->buffer,256);
			strcpy(clientObj->buffer,"closefile");
			std::cout<<"Send close command :"<<clientObj->buffer<<std::endl;
			Error = clientObj->ClientWrite();
			//acknowlegde received command
			Error = clientObj->ClientRead();
			if(Error != SUCCESS)
			{
				PrintError("Client Unable to read from server");
			}
			break;
		}
	}
	Error = FileObj.CloseFile();
}

void DownloadFile(CClient *clientObj,std::string FileName)
{
	eErrorType Error = SUCCESS;
        CFileReading FileObj;
	Error = FileObj.OpenUpdown(FileName);
	while(Error == SUCCESS)
	{
		//Received File data from server
		Error = clientObj->ClientRead();
		if(Error != SUCCESS)
		{
			PrintError("Client Unable to read from server");
			break;
		}
		if( strcmp(clientObj->buffer,"closefile") != 0)
                {
                        bzero(FileObj.buff,256);
                        bcopy(clientObj->buffer,FileObj.buff,255);
                        Error = FileObj.WriteFile(255);
                        std::cout<<"Data writen to file :"<<FileObj.buff<<std::endl;
                }

		//acknowledge received file data to server
		Error = clientObj->ClientWrite();
		if(Error != SUCCESS)
		{
			PrintError("Unable to Write to server from client");
		}
		if(strcmp(clientObj->buffer,"closefile") == 0)
		{
			std::cout<<"Received closefile command :"<<clientObj->buffer<<std::endl;
			Error = clientObj->ClientWrite();
                        Error = FileObj.CloseFile();
                        break;
		}
	}
	Error = FileObj.CloseFile();
}

void SendRemove(CClient *clientObj)
{
	//acknowlegde received command
	eErrorType Error = SUCCESS;
	Error = clientObj->ClientRead();
	if(Error != SUCCESS)
	{
		PrintError("Client Unable to read from server");
	}
	std::cout<<"Message Received From server :"<<clientObj->buffer<<std::endl;
}

int main()
{
	CClient clientObj;
	CFileReading FileObj;
	char hostbuffer[256];
	eErrorType Error = SUCCESS;
	std::string Filename;
	int choice = 0;
	choice = menu();
	std::string cmd = CreateCommand(choice);
	std::cout<<"Enter File name : ";
	std::cin>>Filename;



	Error = clientObj.OpenSocket();	
	if(Error != SUCCESS)
	{
		PrintError("Client Open Socket Fail");
	}
	bzero(hostbuffer,256);
	int hostnameRet = gethostname(hostbuffer, sizeof(hostbuffer));
	if(hostnameRet < 0)
	{
		PrintError("Unable to local machine get Host Name");
	}
	std::cout<<hostbuffer<<std::endl;

	Error = clientObj.GetHost(hostbuffer);
	if(Error != SUCCESS)
	{
		PrintError("Unable to get Host from client");
	}

	Error = clientObj.ClientConnect();
	if(Error != SUCCESS)
	{
		PrintError("Unable to Connect to server");
	}
	SendCommand(&clientObj,cmd);

	SendFilename(&clientObj,Filename);
	if(cmd.compare("Rename") == 0)
	{
		SendRename(&clientObj);
	}
	else if(cmd.compare("delete") == 0)
	{
		SendRemove(&clientObj);
	}
	else if(cmd.compare("upload") == 0)
	{
		UploadFile(&clientObj,Filename);
	}
	else if(cmd.compare("download") == 0)
	{
		DownloadFile(&clientObj,Filename);
	}
	
	clientObj.CloseSocket();
	
	return 0;
}
