//podiceps - pocket dictonary

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "DFile.H"
#include "DHandler.H"

#define _(STR) (STR)
#define N_(STR) (STR)

int main(int argc, char** argv)
{
	ParsedStr::SetPaths();
	if(argc == 1) {
		//GUI
	}
	else
		arg_handling(argc, argv);
	return 0;
}
