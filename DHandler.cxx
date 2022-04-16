#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "DFile.H"
#include "DHandler.H"

const char dest_file_path[] = "files/test.txt";
const char temp_file_path[] = "files/temp.txt";

void help_page()
{
	printf("Options:\n");
	printf("-h - Show this help.\n"); 
	printf("-a [original] [translation] - Add new word.\n"); 
	printf("-d [original] - Delete word by original.\n"); 
	printf("-O [old original] [new original] - To correct the original.\n");
	printf("-T [original] [new_translation]- To correct the translation.\n"); 
	printf("-T [original] - Change status.\n"); 
	printf("-p [pattern] - Show all words matching the pattern.\n"); 
	printf("Without arguments - To run GUI.\n"); 
}

void add_word(const char* ostr, const char* tstr)
{
	ParsedStr ps(ostr, tstr);
	ps.AddStringToFile(dest_file_path, temp_file_path);
}

void delete_word(const char* word)
{
	ParsedStr ps;
	ps.ChangeOriginal(word);
	ps.DeleteStringFromFile(dest_file_path, temp_file_path);
}

void change_original(const char* old_word, const char* new_word)
{
	ParsedStr oldp;
	oldp.ChangeOriginal(old_word);
	const char *olds = oldp.FindByOriginal(dest_file_path);
	if(olds[0] == '\0') {
		printf("Word %s not founded.\n", old_word);
		exit(1);
	}
	oldp.DeleteStringFromFile(dest_file_path, temp_file_path);
	ParsedStr newp(olds);
	newp.ChangeOriginal(new_word);
	newp.AddStringToFile(dest_file_path, temp_file_path);
}

void change_translation(const char* word, const char* new_translation)
{
	ParsedStr ps(word, new_translation);
	ps.ReplaceByOriginalInFile(dest_file_path, temp_file_path);
	ps.ChangeTranslation(new_translation);
	ps.ReplaceByOriginalInFile(dest_file_path, temp_file_path);
}

void change_status(const char* word)
{
	ParsedStr ps;
	ps.ChangeOriginal(word);
	const char *s = ps.FindByOriginal(dest_file_path);
	if(s[0] == '\0') {
		printf("Word %s not founded.\n", word);
		exit(1);
	}
	ps.DeleteStringFromFile(dest_file_path, temp_file_path);
	ParsedStr nps(s);
	nps.ChangeStatus();
	nps.AddStringToFile(dest_file_path, temp_file_path);
}

void show_words(const char* pttr)
{
	DFile cur;
	cur.OpenR(dest_file_path);
	char buf[SrcStr::srclen];
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
