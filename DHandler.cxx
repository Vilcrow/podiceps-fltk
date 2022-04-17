#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "DFile.H"
#include "DHandler.H"

enum action { a_add, a_del, a_cor, a_show }; //what need to do
enum reqpart { rp_origl, rp_tranl, rp_st, rp_dt }; //required part

extern char *paths[3];

void help_page()
{
	printf("Options:\n");
	printf("-h - Show this help.\n"); 
	printf("-a [original] [translation] - Add new word.\n"); 
	printf("-d [original] - Delete word by original.\n"); 
	printf("-O [old original] [new original] - To correct the original.\n");
	printf("-T [original] [new_translation]- To correct the translation.\n"); 
	printf("-S [original] - Change status.\n"); 
	printf("-p [pattern] - Show all words matching the pattern.\n"); 
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
	ps.ChangeOriginal(word);
	ps.DeleteStringFromFile();
}

void change_original(const char* old_word, const char* new_word)
{
	ParsedStr oldp;
	oldp.ChangeOriginal(old_word);
	const char *olds = oldp.FindByOriginal();
	if(olds[0] == '\0') {
		printf("Word \"%s\" not found.\n", old_word);
		exit(1);
	}
	oldp.DeleteStringFromFile();
	ParsedStr newp(olds);
	newp.ChangeOriginal(new_word);
	newp.AddStringToFile();
}

void change_translation(const char* word, const char* new_translation)
{
	ParsedStr ps(word, new_translation);
	ps.ReplaceByOriginalInFile();
	ps.ChangeTranslation(new_translation);
	ps.ReplaceByOriginalInFile();
}

void change_status(const char* word)
{
	ParsedStr ps;
	ps.ChangeOriginal(word);
	const char *s = ps.FindByOriginal();
	if(s[0] == '\0') {
		printf("Word \"%s\" not found.\n", word);
		exit(1);
	}
	ps.DeleteStringFromFile();
	ParsedStr nps(s);
	nps.ChangeStatus();
	nps.AddStringToFile();
}

void show_words(const char* pttr)
{
	DFile cur;
	cur.OpenR(paths[0]);
	char buf[ParsedStr::srclen];
	bool done = false;
	ParsedStr ps;
	while(fgets(buf, sizeof(buf), cur.GetFl())) {
		ps = buf;
		if(is_match(ps.GetOriginal(), pttr)) {
			show_word(buf);
			done = true;
		}
	}
	if(!done) {
		printf("Requested word don't exist in dictonary.\n");
		exit(0);
	}
}

void show_word(const char* string)
{
	ParsedStr ps(string);
	printf("[%s] - [%s] - [%s] - [%s]\n", ps.GetOriginal(),
			ps.GetTranslation(), ps.GetStatus(), ps.GetDate());
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

void arg_handling(int argc, char **argv)
{
	action act;
	reqpart rp;
	char *arg1;
	char *arg2;
	int opt;
	opterr = 0; //getopt doesn't print any messages
	while((opt = getopt(argc, argv, "ha:d:O:T:S:p:")) != -1) {
		switch(opt) {
		case 'h':
			help_page();
			exit(0);
		case 'a':
			act = a_add;
			arg1 = optarg;
			arg2 = argv[optind];
			break;
		case 'd':
			act = a_del;
			arg1 = optarg;
			break;
		case 'O':
			act = a_cor;
			rp = rp_origl;
			arg1 = optarg;
			arg2 = argv[optind];
			break;
		case 'T':
			act = a_cor;
			rp = rp_tranl;
			arg1 = optarg;
			arg2 = argv[optind];
			break;
		case 'S':
			act = a_cor;
			rp = rp_st;
			arg1 = optarg;
			break;
		case 'p':
			act = a_show;
			arg1 = optarg;
			break;
		default:
			help_page();
			exit(1);
		}
	}
	switch(act) {
	case a_add:
		add_word(arg1, arg2);
		break;
	case a_del:
		delete_word(arg1);
		break;
	case a_cor:
		if(rp == rp_origl)
			change_original(arg1, arg2);
		else if(rp == rp_tranl)
			change_translation(arg1, arg2);
		else if(rp == rp_st)
			change_status(arg1);
		break;
	case a_show:
		show_words(arg1);
	}
}
