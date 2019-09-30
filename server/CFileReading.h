#ifndef FILEREADING_H
#define FILEREADING_H

#include<string>
#include<vector>
#include<map>
#include<cstring>
#include<fstream>
#include"Utility.h"


class CFileReading
{
	public:
		eErrorType OpenFile(std::string FileName);
		eErrorType OpenUpdown(std::string FileName);
		eErrorType CloseFile();
		eErrorType ReadFile(int *readsize);
		eErrorType WriteFile(int size=256);
		eErrorType RemoveFile(std::string Filename);
		eErrorType RenameFile(std::string OldFilename,std::string NewFilename);
		char buff[256];	
	private:
		std::string FileName;
		std::fstream FileStream;
};

#endif
