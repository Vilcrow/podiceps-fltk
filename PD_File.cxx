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
		//!need add the warning message
		//"Warning: element %d not exist. Returned end of string.", ind
		int i;
		while(s[i] != '\0'){ //return end of string
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
	while(s[i] != '\0') { //find end of "s"
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
	if(string[j] != '\0') {
		//!nedd add the warning message
	}
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
	if(a[j] != '\0') {
		//!nedd add the warning message
	}
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
	original = orl;
	translate = trl;
	st = n;
	char* tmp = GetCurrentDate();
	strcpy(date, tmp);
}

PD_Card::PD_Card(PD_String& orl, PD_String& trl)
{
	original = new PD_String();
	translate = new PD_String();
	*original = orl;
	*translate = trl;
	st = n;
	char* tmp = GetCurrentDate();
	strcpy(date, tmp);
}

PD_Card::PD_Card(const char* orl, const char* trl)
{
	original = new PD_String();
	translate = new PD_String();
	*original = orl;
	*translate = trl;	
	st = n;
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
	fprintf(fl, "[%s]", date);
	fputs( "\n", fl);
}

char* PD_Card::GetCurrentDate() const
{
	char* date = new char[size_date];
	struct tm *tmp;
	const time_t timer = time(NULL);
	tmp = localtime(&timer);
	strftime(date, size_date, "%d-%m-%Y", tmp);
	date[size_date] = '\0';
	return date;
}

void PD_Card::SetDate(const char* str)
{
	strcpy(date, str);
}

PD_Card::PD_Card(const char* string)
{
	int i = 1;
	int j = 0;
	char tmp[101];
	char* ptmp = tmp;
	while(string[i] != ')' && j < 101) {
		tmp[j] = string[i];	
		++i;
		++j;
	}
	tmp[j] = '\0';
	original = new PD_String();
	*original = ptmp;
	++i;
	++i;
	j = 0;
	while(string[i] != '|' && j < 101) {
		tmp[j] = string[i];	
		++i;
		++j;
	}
	tmp[j] = '\0';
	translate = new PD_String();
	*translate = ptmp;
	++i;
	++i;
	j = 0;
	while(string[i] != '}' && j < 101) {
		tmp[j] = string[i];	
		++i;
		++j;
	}
	if(strcmp(tmp, "new"))
		st = n;
	else
		st = r;
	++i;
	++i;
	j = 0;
	while(string[i] != ']') {
		date[j] = string[i];
		++i;
		++j;
	}
	date[j] = '\0';
}

void PD_Card::SetOriginal(const char* from)
{
	delete original;
	original = new PD_String(from);
}

void PD_Card::SetTranslate(const char* from)
{
	delete translate;
	translate = new PD_String(from);
}

PD_String::PD_String(PD_String& from)
{
	int i = 0;
	while(from[i] != '\0') {
		s[i] = from[i];
		++i;
	}
	s[i] = '\0';
	len = from.GetLen();
}

PD_Card::PD_Card()
{
	original = new PD_String();
	translate = new PD_String();
	st = n;
	SetDate(GetCurrentDate());
}

char* PD_Card::MakeString() const
{
	char* tmp = new char [101];
	int i = 0;
	tmp[i++] = '(';
	int j = 0;
	while(original->GetString()[j] != '\0') {
		tmp[i++] = original->GetString()[j++];
	}
	tmp[i++] = ')';
	tmp[i++] = '|';
	j = 0;
	while(translate->GetString()[j] != '\0') {
		tmp[i++] = translate->GetString()[j++];
	}
	tmp[i++] = '|';
	tmp[i++] = '{';
	if(st == 0) { //need fix
		tmp[i++] = 'n';
		tmp[i++] = 'e';
		tmp[i++] = 'w';
	}
	else {
		tmp[i++] = 'r';
		tmp[i++] = 'e';
		tmp[i++] = 'm';
		tmp[i++] = 'e';
		tmp[i++] = 'm';
		tmp[i++] = 'b';
		tmp[i++] = 'e';
		tmp[i++] = 'r';
		tmp[i++] = 'e';
		tmp[i++] = 'd';
	}
	tmp[i++] = '}';
	j = 0;
	tmp[i++] = '[';
	while(date[j] != '\0') {
		tmp[i++] = date[j++];
	}
	tmp[i++] = ']';
	tmp[i++] = '\n';
	tmp[i] = '\0';
	return tmp;
	
}
