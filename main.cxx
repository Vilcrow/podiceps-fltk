//podiceps - pocket dictonary

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include "PD_File.H"

int main(int argc, char** argv)
{
	PD_String str1(argv[1]);
	PD_String str2(argv[2]);
	PD_Card card(str1, str2);
	PD_File* file = new PD_File();
	file->OpenRW("test.txt");
	file->AddString(card);
	return 0;
}
