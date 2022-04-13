//podiceps - pocket dictonary

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "PD_File.H"
#include "PD_Handler.H"

#define _(STR) (STR)
#define N_(STR) (STR)

int main(int argc, char** argv)
{
	if(argc == 1) {
		//GUI
	}
	else {
		int opt;
		opterr = 0;
		while((opt = getopt(argc, argv, "ha:d:O:T:S:p:")) != -1) {
			switch(opt) {
			case 'h':
				help_page();
				exit(0);
			case 'a':
				add_word(optarg, argv[optind]);
				exit(0);
			case 'd':
				delete_word(optarg);
				exit(0);
			case 'O':
				change_original(optarg, argv[optind]);
				exit(0);
			case 'T':
				change_translate(optarg, argv[optind]);
				exit(0);
			case 'S':
				change_status(optarg);
				exit(0);
			case 'p':
				show_words(optarg);
				exit(0);
			default:
				help_page();
				exit(1);
			}
		}
	}
	return 0;
}
