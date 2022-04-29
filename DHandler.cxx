/*
podiceps - pocket dictonary

Copyright (C) 2022 S.V.I 'Vilcrow', <www.vilcrow.net>
--------------------------------------------------------------------------------
LICENCE:
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.
--------------------------------------------------------------------------------
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "DFile.H"
#include "DHandler.H"

extern char *paths[3];

void help_page()
{
	printf("Options:\n");
	printf("-h - Show this help.\n"); 
	printf("-a [original] [translation] - Add new word.\n"); 
	printf("-d [original] - Delete word by original.\n"); 
	printf("-O [old original] [new original] - To correct the original.\n");
	printf("-T [original] [new translation]- To correct the translation.\n"); 
	printf("-S [original] [new status] - Change status.\n"); 
	printf("-o [original],\n"); 
	printf("-t [translation],\n"); 
	printf("-s [status],\n"); 
	printf("-D [date] - Show all words matching the pattern. You can use \"*\" and \"?\".\n"); 
	printf("Without arguments - To run GUI.\n"); 
}

void add_word(const char* ostr, const char* tstr)
{
	ParsedStr ps(ostr, tstr);
	ps.AddStringToFile();
}

void delete_word(const char* word)
{
	ParsedStr ps;
	ps.Original(word);
	ps.DeleteStringFromFile();
}

void change_original(const char* old_word, const char* new_word)
{
	ParsedStr oldp;
	oldp.Original(old_word);
	const char *olds = oldp.FindByOriginal();
	if(olds[0] == '\0') {
		printf("Word \"%s\" not found.\n", old_word);
		exit(1);
	}
	oldp.DeleteStringFromFile();
	ParsedStr newp(olds);
	newp.Original(new_word);
	newp.AddStringToFile();
}

void change_translation(const char* word, const char* new_translation)
{
	ParsedStr ps(word, new_translation);
	ps.ReplaceByOriginalInFile();
	ps.Translation(new_translation);
	ps.ReplaceByOriginalInFile();
}

void change_status(const char* word, const char *ns)
{
	ParsedStr ps;
	ps.Original(word);
	const char *s = ps.FindByOriginal();
	if(s[0] == '\0') {
		printf("Word \"%s\" not found.\n", word);
		exit(1);
	}
	ps.DeleteStringFromFile();
	ParsedStr nps(s);
	nps.WStatus(ns);
	nps.AddStringToFile();
}

void show_words(const char* pttr, const enum reqpart rp)
{
	DFile cur;
	cur.OpenR(paths[0]);
	char buf[ParsedStr::srclen];
	char tmp[ParsedStr::orglen];
	bool done = false;
	ParsedStr ps;
	while(fgets(buf, sizeof(buf), cur.GetFl())) {
		ps = buf;
		switch(rp) {
		case rp_origl:
			strcpy(tmp, ps.Original());
			break;
		case rp_tranl:
			strcpy(tmp, ps.Translation());
			break;
		case rp_st:
			strcpy(tmp, ps.WStatus());
			break;
		case rp_dt:
			strcpy(tmp, ps.Date());
			break;
		}
		if(is_match(tmp, pttr)) {
			print_word(buf);
			done = true;
		}
	}
	if(!done) {
		printf("Requested word don't exist in dictonary.\n");
		exit(0);
	}
}

void print_word(const char* string)
{
	ParsedStr ps(string);
	printf("%s\n", ps.Original());
	printf("%s\n", ps.Translation());
	printf("[%s] ", ps.WStatus());
	printf("[%s]\n", ps.Date());
	printf("***********************************\n");
}

bool is_match(const char* string, const char* pattern)
{
	int i;
	for(;; string++, pattern++) {
		switch(*pattern) {
			case 0:
				return *string == 0;
			case '*':
				for(i = 0; ; i++) {
					if(is_match(string+i, pattern+1))
						return true;
					if(!string[i])
						return false;
				}
			case '?':
				if(!*string)
					return false;
				break;
			default:
				if(*string != *pattern)
					return false;
		}
	}
}

cl_arg* get_arguments(int argc, char **argv)
{
	cl_arg *cla = new cl_arg;
	int opt;
	opterr = 0; //getopt doesn't print any messages
	while((opt = getopt(argc, argv, "ha:d:O:T:S:o:t:s:D:c")) != -1) {
		switch(opt) {
		case 'h':
			help_page();
			exit(0);
		case 'a':
			cla->act = a_add;
			cla->arg1 = optarg;
			cla->arg2 = argv[optind];
			break;
		case 'd':
			cla->act = a_del;
			cla->arg1 = optarg;
			break;
		case 'O':
			cla->act = a_cor;
			cla->rp = rp_origl;
			cla->arg1 = optarg;
			cla->arg2 = argv[optind];
			break;
		case 'T':
			cla->act = a_cor;
			cla->rp = rp_tranl;
			cla->arg1 = optarg;
			cla->arg2 = argv[optind];
			break;
		case 'S':
			cla->act = a_cor;
			cla->rp = rp_st;
			cla->arg1 = optarg;
			cla->arg2 = argv[optind];
			break;
		case 'o':
			cla->act = a_show;
			cla->arg1 = optarg;
			cla->rp = rp_origl;
			break;
		case 't':
			cla->act = a_show;
			cla->rp = rp_tranl;
			cla->arg1 = optarg;
			break;
		case 's':
			cla->act = a_show;
			cla->rp = rp_st;
			cla->arg1 = optarg;
			break;
		case 'D':
			cla->act = a_show;
			cla->rp = rp_dt;
			cla->arg1 = optarg;
			break;
		case 'c':
			printf("Total count: %d\n", word_count());
			exit(0);
		default:
			help_page();
			exit(1);
		}
	}
	return cla;
}

void handle_arguments(const cl_arg *cla)
{
	switch(cla->act) {
	case a_add:
		add_word(cla->arg1, cla->arg2);
		break;
	case a_del:
		delete_word(cla->arg1);
		break;
	case a_cor:
		if(cla->rp == rp_origl)
			change_original(cla->arg1, cla->arg2);
		else if(cla->rp == rp_tranl)
			change_translation(cla->arg1, cla->arg2);
		else if(cla->rp == rp_st)
			change_status(cla->arg1, cla->arg2);
		break;
	case a_show:
		show_words(cla->arg1, cla->rp);
	}
}

int word_count()
{
	DFile dct;
	dct.OpenR(paths[0]);
	int c = 0;
	char buf[ParsedStr::srclen];
	while(fgets(buf, sizeof(buf), dct.GetFl())) {
		++c;
	}
	return c;
}
