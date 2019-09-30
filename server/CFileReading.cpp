#include"CFileReading.h"
#include<stdio.h>
#include<stdlib.h>
#include<iostream>
#include<algorithm>
#include<strings.h>


eErrorType CFileReading::OpenFile(std::string FileName)
{
	eErrorType Error = SUCCESS;
	this->FileName = FileName;
	FileStream.open(this->FileName.c_str(),std::fstream::in | std::fstream::out | std::fstream::binary);
	if(!(FileStream.is_open()))
	{
		std::cout<<"File Error : "<<FileName<<std::endl;
		perror ("The following error occurred");
		Error = FILEOPENFAIL;
	}
	return Error;
}

eErrorType CFileReading::OpenUpdown(std::string FileName)
{
	eErrorType Error = SUCCESS;
	this->FileName = FileName;
	FileStream.open(this->FileName.c_str(),std::fstream::in | std::fstream::out | std::fstream::trunc | std::fstream::binary);
	if(!(FileStream.is_open()))
	{
		std::cout<<"File Error : "<<FileName<<std::endl;
		perror ("The following error occurred");
		Error = FILEOPENFAIL;
	}
	return Error;
}


eErrorType CFileReading::CloseFile()
{
	eErrorType Error = SUCCESS;
	FileStream.close();
	return Error;
}
eErrorType CFileReading::ReadFile(int *readsize)
{
	eErrorType Error = SUCCESS;
	bzero(this->buff, 256);
	if(!FileStream.eof())
	{
		FileStream.read(this->buff,255);
		*readsize = FileStream.gcount();
	}
	else
	{
		std::cout<<"File End"<<std::endl;
		Error = ENDOFFILE;
	}
	
	return Error;
}


eErrorType CFileReading::WriteFile(int size)
{
	eErrorType Error = SUCCESS;
	if(FileStream.is_open())
	{
		FileStream.write(this->buff,size);
	}
	
	return Error;
}

eErrorType CFileReading::RemoveFile(std::string Filenames)
{
	eErrorType Error = SUCCESS;
	std::cout<<"Removing FIle : "<<Filenames<<std::endl;
	std::cout<<"Removing FIle : "<<Filenames.length()<<std::endl;
	//int RemoveRet = remove("file");	
	int RemoveRet = remove(Filenames.c_str());	
	if(RemoveRet !=0)
	{
		std::cout<<"Delete error"<<std::endl;
		perror ("The following error occurred");
		Error = DELETEFAIL;
	}
	return Error;
}
eErrorType CFileReading::RenameFile(std::string OldFilename,std::string NewFilename)
{
	eErrorType Error = SUCCESS;
	int reNameRe = rename(OldFilename.c_str(),NewFilename.c_str());	
	if(reNameRe !=0)	
	{
		Error = RENAMEFAIL;	
	}
	return Error;
}
