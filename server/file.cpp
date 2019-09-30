#include<iostream>
#include<fstream>


int main()
{
	std::fstream FileStream;
	char buff[4];
	FileStream.open("Movie.txt",std::fstream::in | std::fstream::out | std::ifstream::binary);
	if(!(FileStream.is_open()))
        {
                std::cout<<"File Error : "<<std::endl;
        }

	while(!FileStream.eof())
	{
		FileStream.read(buff,4);
		std::cout<<buff;
	}
	FileStream.close();
	return 0;
}
