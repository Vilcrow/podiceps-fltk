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
	PD_Card card(ostr, tstr);
	put_new_string(card, TEST, TEMP);
}

void delete_word(const char* word)
{
	PD_File cur;
	PD_File temp;
	cur.OpenR(TEST);
	temp.OpenW(TEMP);
	char buf[101];
	bool done = false;
	while(fgets(buf, sizeof(buf), cur.GetFl())) {
		if(!done) {
			if(!find_by_original(word, buf))
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
	cur.Close();
	temp.Close();
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
	bool done = false;
	while(fgets(buf, sizeof(buf), cur.GetFl())) {
		if(!done) {
			if(find_by_original(old_word, buf)) {
				strcpy(nw, buf);
				done = true;
			}
			else
				fputs(buf, temp.GetFl());
		}
		else
			fputs(buf, temp.GetFl());
	}
	cur.Close();
	temp.Close();
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
	int code;
	code = rename(TEMP, TEST);
	if(code != 0) {
		printf("Renaming failed.\n");
		exit(1);
	}
	PD_Card new_card(pnw);
	new_card.SetOriginal(new_word);
	put_new_string(new_card, TEST, TEMP);
}

void change_translate(const char* word, const char* new_translate)
{
	PD_File cur;
	PD_File temp;
	cur.OpenR(TEST);
	temp.OpenW(TEMP);
	char buf[101];
	bool done = false;
	while(fgets(buf, sizeof(buf), cur.GetFl())) {
		if(!done) {
			if(find_by_original(word, buf)) {
				PD_Card new_card(buf);
				new_card.SetTranslate(new_translate);
				new_card.PrintToFile(temp.GetFl());
				done = true;
			}
			else
				fputs(buf, temp.GetFl());
		}
		else
			fputs(buf, temp.GetFl());
	}
	if(!done) {
		printf("The word don't exist.\n");
		exit(1);
	}
	cur.Close();
	temp.Close();
	int code;
	code = rename(TEMP, TEST);
	if(code != 0) {
		printf("Renaming failed.\n");
		exit(1);
	}
}

void change_status(const char* word, const char* new_status)
{
	PD_File cur;
	PD_File temp;
	cur.OpenR(TEST);
	temp.OpenW(TEMP);
	char buf[101];
	bool done = false;
	PD_Card* card;
	while(fgets(buf, sizeof(buf), cur.GetFl())) {
		if(!done) {
			if(find_by_original(word, buf)) {
				card = new PD_Card(buf);	
				card->ChangeStatus();
				card->PrintToFile(temp.GetFl());
				done = true;
			}
			else
				fputs(buf, temp.GetFl());
		}
		else
			fputs(buf, temp.GetFl());
	}
	cur.Close();
	temp.Close();
	int code;
	code = rename(TEMP, TEST);
	if(code != 0) {
		printf("Renaming failed.\n");
		exit(1);
	}
}

void view_word()
{

}

bool find_by_original(const char* original, const char* string)
{
	int i = 0;
	int j = 0;
	char tmp[101];
	while(string[i++] != '(') {
		;
	}	
	while(string[i] != ')') {
		tmp[j++] = string[i++];	
	}
	tmp[j] = '\0';
	return (strcmp(original, tmp) ? false : true);
}

void put_new_string(const PD_Card str, const char* dest, const char* temp)
{
	char buf[101];
	char *card = str.MakeString();
	int cmp;
	bool done = false;
	PD_File cur;
	PD_File tmp;
	cur.OpenR(dest);
	tmp.OpenW(temp);
	while(fgets(buf, sizeof(buf), cur.GetFl())) {
		if(!done) {
			cmp = strcmp(card, buf);
			if(cmp < 0) {
				fputs(card, tmp.GetFl());
				fputs(buf, tmp.GetFl());
				done = true;
			}
			else if(cmp == 0) {
				printf("Word already exists.\n");
				exit(1);
			}
			else
				fputs(buf, tmp.GetFl());
		}
		else
			fputs(buf, tmp.GetFl());
	}
	cur.Close();
	tmp.Close();
	int code;
	code = rename(temp, dest);
	if(code != 0) {
		printf("Renaming failed.\n");
		exit(1);
	}
}
