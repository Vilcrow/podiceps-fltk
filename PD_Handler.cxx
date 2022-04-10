#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "PD_File.H"
#include "PD_Handler.H"

void help_page()
{
	printf("help\n");
}

void add_word(const char* ostr, const char* tstr)
{
	PD_String ornl(ostr);
	PD_String trnl(tstr);
	PD_Card card(ornl, trnl);
	PD_File cur; //current vocabulary
	PD_File temp; //temporary file
	cur.OpenR(TEST);
	temp.OpenW(TEMP);
	char buf[101];
	char original[101];
	bool done = false;
	int i = 1;
	int j = 0;
	while(fgets(buf, sizeof(buf), cur.GetFl())) {
		if(!done) {
			if(buf[0] != '(') {
				printf("Incorrect string in vocabulary file:\n %s", buf);
				exit(1);
			}
			while(buf[i] != ')') {
				original[j] = buf[i];	
				++i;
				++j;
			}
			original[j] = '\0';
			i = 1;
			j = 0;
			int cmp = strcmp(ostr, original);
			if(cmp < 0) {
				card.PrintToFile(temp.GetFl());
				done = true;
			}
			else if(cmp == 0) {
				printf("Word already exists.\n");
				exit(1);
			}
		}
		fputs(buf, temp.GetFl());
	}
	if(!done)
		card.PrintToFile(temp.GetFl());
	int code;
	code = rename(TEMP, TEST);
	if(code != 0) {
		printf("Renaming failed.\n");
		exit(1);
	}
}

void delete_word(const char* word)
{
	PD_File cur;
	PD_File temp;
	cur.OpenR(TEST);
	temp.OpenW(TEMP);
	char buf[101];
	char original[101];
	bool done = false;
	int i = 1;
	int j = 0;
	while(fgets(buf, sizeof(buf), cur.GetFl())) {
		if(!done) {
			if(buf[0] != '(') {
				printf("Incorrect string in vocabulary file:\n %s", buf);
				exit(1);
			}
			while(buf[i] != ')') {
				original[j] = buf[i];	
				++i;
				++j;
			}
			original[j] = '\0';
			i = 1;
			j = 0;
			int cmp = strcmp(word, original);
			if(cmp != 0)
				fputs(buf, temp.GetFl());
			else
				done = true;
		}
		else
			fputs(buf, temp.GetFl());
	}
	if(!done) {
		printf("The word don't exist.\n");
		exit(1);
	}
	int code;
	code = rename(TEMP, TEST);
	if(code != 0) {
		printf("Renaming failed.\n");
		exit(1);
	}
}

void change_original(const char* old_word, const char* new_word)
{
	PD_File cur;
	PD_File temp;
	cur.OpenR(TEST);
	temp.OpenW(TEMP);
	char buf[101];
	char nw[101];
	char* pnw = nw;
	nw[0] = '\0';
	char original[101];
	bool done = false;
	int i = 1;
	int j = 0;
	int cmp;
	while(fgets(buf, sizeof(buf), cur.GetFl())) {
		if(!done) {
			if(buf[0] != '(') {
				printf("Incorrect string in vocabulary file:\n %s", buf);
				exit(1);
			}
			while(buf[i] != ')') {
				original[j] = buf[i];	
				++i;
				++j;
			}
			original[j] = '\0';
			i = 1;
			j = 0;
			cmp = strcmp(old_word, original);
			if(cmp == 0) { //the original word finded
				strcpy(nw, buf);
				done = true;
			}
			else
				fputs(buf, temp.GetFl());
		}
		else
			fputs(buf, temp.GetFl());
	}
	if(!done) {
		int code;
		code = rename(TEMP, TEST);
		if(code != 0) {
			printf("Renaming failed.\n");
			exit(1);
		}
		printf("The word don't exist.\n");
		exit(1);
	}
	done = false; //need fix
	cur.Close();
	temp.Close();
	cur.OpenR(TEMP);
	temp.OpenW(TEST);
	PD_Card new_card(pnw);
	new_card.SetOriginal(new_word);
	while(fgets(buf, sizeof(buf), cur.GetFl())) {
		if(!done) {
			if(buf[0] != '(') {
				printf("Incorrect string in vocabulary file:\n %s", buf);
				exit(1);
			}
			while(buf[i] != ')') {
				original[j] = buf[i];	
				++i;
				++j;
			}
			original[j] = '\0';
			i = 1;
			j = 0;
			cmp = strcmp(new_word, original);
			if(cmp < 0) {
				new_card.PrintToFile(temp.GetFl());
				fputs(buf, temp.GetFl());
				done = true;
			}
			else if(cmp == 0) {
				printf("Word already exists.\n");
				exit(1);
			}
			else
				fputs(buf, temp.GetFl());
		}
		else
			fputs(buf, temp.GetFl());
	}
}

void change_translate(const char* old_word, const char* new_word)
{

}

void change_status(const char* old_word, const char* new_word)
{

}

void view_word()
{

}
