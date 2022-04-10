#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include "PD_File.H"

void PD_File::OpenR(const char* name)
{
	fl = fopen(name, "r");
	if(!fl) {
		perror(name); //need fix
		exit(1);
	}
}

void PD_File::OpenRP(const char* name)
{
	fl = fopen(name, "r+");
	if(!fl) {
		perror(name); //need fix
		exit(1);
	}
}

void PD_File::OpenW(const char* name)
{
	fl = fopen(name, "w");
	if(!fl) {
		perror(name); //need fix
		exit(1);
	}
}

void PD_File::OpenWP(const char* name)
{
	fl = fopen(name, "w+");
	if(!fl) {
		perror(name); //need fix
		exit(1);
	}
}

void PD_File::OpenA(const char* name)
{
	fl = fopen(name, "a");
	if(!fl) {
		perror(name); //need fix
		exit(1);
	}
}

void PD_File::OpenAP(const char* name)
{
	fl = fopen(name, "a+");
	if(!fl) {
		perror(name); //need fix
		exit(1);
	}
}

PD_String& PD_String::operator=(const char* from)
{
	int i = 0;
	if(from) {
		while(from[i] != '\0' && i < max-2) {
			s[i] = from[i];
			++i;
		}
		s[i] = '\0';
		len = i;
	}
	else {
		s[0] = '\0'; //string is empty
		len = 0;
	}
	return *this;
}

char& PD_String::operator[](int ind)
{
	if(ind >= 0 && ind < len)
		return s[ind];
	else {
		int i;
		while(s[i] != '\0'){
			++i;
		}
		return s[i];
	}
}

PD_String& PD_String::operator=(PD_String& f)
{
	int i = 0;
	while(f[i] != '\0') {
		s[i] = f[i];
		++i;
	}
	len = i;
	s[++i] = '\0';
	return *this;
}

PD_String& PD_String::operator+(const char* string)
{
	int i = 0;
	while(s[i] != '\0') { //find end of current string
		++i;
	}
	int j = 0;
	while(string[j] != '\0' && i < max-2){
		s[i] = string[j];
		++i;
		++j;
	}
	s[i] = '\0';
	len = i;
	return *this;
}

PD_String& PD_String::operator+(PD_String& a)
{
	int i = 0;
	while(s[i] != '\0') { //find end of current string
		++i;
	}
	int j = 0;
	while(a[j] != '\0' && i < max-2){
		s[i] = a[j];
		++i;
		++j;
	}
	s[i] = '\0';
	len = i;
	return *this;
}

//delete last n characters from array
PD_String& PD_String::operator-(const int n)
{
	int i = len - n;
	if(i <= 0) {
		s[0] = '\0';
		len = 0;
	}
	else {
		s[i] = '\0';
		len = i;
	}
	return *this;
}

PD_String& PD_String::operator+=(const char* string)
{
	*this = *this + string;
	return *this;
}

PD_String& PD_String::operator+=(PD_String& f)
{
	*this = *this + f;
	return *this;
}

PD_String& PD_String::operator-=(const int n)
{
	*this = *this - n;
	return *this;	
}

PD_Card::PD_Card(PD_String* orl, PD_String* trl)
{
	st = n;
	original = orl;
	translate = trl;
	char* tmp = GetCurrentDate();
	strcpy(date, tmp);
}

PD_Card::PD_Card(PD_String& orl, PD_String& trl)
{
	st = n;
	original = new PD_String();
	translate = new PD_String();
	*original = orl;
	*translate = trl;
	char* tmp = GetCurrentDate();
	strcpy(date, tmp);
}

PD_Card::PD_Card(char* orl, char* trl)
{
	st = n;
	original = new PD_String();
	translate = new PD_String();
	*original = orl;
	*translate = trl;	
	char* tmp = GetCurrentDate();
	strcpy(date, tmp);
}

static const char *const status[] = { "new", "remembered" };

void PD_Card::PrintToFile(FILE* fl) const
{
	fprintf(fl, "(%s)", GetOrlString());
	fprintf(fl, "|%s|", GetTrlString());
	if(GetStatus())
		fprintf(fl, "{%s}", status[1]);
	else
		fprintf(fl, "{%s}", status[0]);
	const char* date = GetCurrentDate();
	fprintf(fl, "[%s]", date);
	fputs( "\n", fl);
}

char* PD_Card::GetCurrentDate() const
{
	char* date = (char*)malloc(size_data);
	struct tm *tmp;
	const time_t timer = time(NULL);
	tmp = localtime(&timer);
	strftime(date, size_data, "%d-%m-%Y", tmp);
	date[size_data] = '\0';
	return date;
}

void PD_Card::SetDate(const char* str)
{
	strcpy(date, str);
}

PD_Card::PD_Card(const char* string)
{
	*this = PD_Card();
	int i = 1;
	int j = 0;
	char tmp[101];
	char* ptmp = tmp;
	while(string[i] != ')') {
		tmp[j] = string[i];	
		++i;
		++j;
	}
	SetOriginal(ptmp);
	++i;
	j = 0;
	while(string[i] != '|') {
		tmp[j] = string[i];	
		++i;
		++j;
	}
	SetTranslate(ptmp);
	++i;
	j = 0;
	while(string[i] != '}') {
		tmp[j] = string[i];	
		++i;
		++j;
	}
	if(strcmp(tmp, "new"))
		st = n;
	else
		st = r;
	++i;
	j = 0;
	while(string[i] != ']') {
		date[j] = string[i];
		++i;
		++j;
	}
}

void PD_Card::SetOriginal(const char* from)
{
	int i = 0;
	while(from[i] != '\0' && i < max_string) {
		original->GetString()[i] = from[i];
		++i;
	}
	original->GetString()[i] = '\0';
}

void PD_Card::SetTranslate(const char* from)
{
	int i = 0;
	while(from[i] != '\0' && i < max_string) {
		translate->GetString()[i] = from[i];
		++i;
	}
	translate->GetString()[i] = '\0';
}
