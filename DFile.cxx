#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include "DFile.H"

const char ParsedStr::st_new[] = "new";
const char ParsedStr::st_rem[] = "remembered";

//class DFile
void DFile::OpenR(const char* name)
{
	fl = fopen(name, "r");
	if(!fl) {
		perror(name); //need fix
		exit(1);
	}
}

void DFile::OpenRP(const char* name)
{
	fl = fopen(name, "r+");
	if(!fl) {
		perror(name); //need fix
		exit(1);
	}
}

void DFile::OpenW(const char* name)
{
	fl = fopen(name, "w");
	if(!fl) {
		perror(name); //need fix
		exit(1);
	}
}

void DFile::OpenWP(const char* name)
{
	fl = fopen(name, "w+");
	if(!fl) {
		perror(name); //need fix
		exit(1);
	}
}

void DFile::OpenA(const char* name)
{
	fl = fopen(name, "a");
	if(!fl) {
		perror(name); //need fix
		exit(1);
	}
}

void DFile::OpenAP(const char* name)
{
	fl = fopen(name, "a+");
	if(!fl) {
		perror(name); //need fix
		exit(1);
	}
}
//class SrcStr
SrcStr::SrcStr(const char *s)
{
	str = new char[srclen];
	strncpy(str, s, srclen);
	if(str[srclen-1] != '\0') { //given too long string
		str[srclen-1] = '\0';
		//!!!print warning message
	}
}

SrcStr::SrcStr(const SrcStr& s)
{
	str = new char[srclen];
	strncpy(str, s.GetPtr(), srclen);
	if(str[srclen-1] != '\0') { //given too long string
		str[srclen-1] = '\0';
		//!!!print warning message
	}
}

void SrcStr::AddStringToFile(const char *destf, const char *tmpf)
{
	DFile dest;
	DFile tmp;
	dest.OpenR(destf);
	tmp.OpenW(tmpf);
	int cmp;
	bool done = false;
	char buf[srclen];
	while(fgets(buf, sizeof(buf), dest.GetFl())) {
		if(!done) {
			cmp = strcmp(str, buf);
			if(cmp < 0) {
				fputs(str, tmp.GetFl());
				fputs(buf, tmp.GetFl());
				done = true;
			}
			else if(cmp == 0) {
				printf("Duplicating a line.\n");
				exit(1);
			}
			else
				fputs(buf, tmp.GetFl());
		}
		else
			fputs(buf, tmp.GetFl());
	}
	if(!done)
		fputs(str, tmp.GetFl());
	dest.Close();
	tmp.Close();
	int code;
	code = rename(tmpf, destf);
	if(code != 0) {
		printf("Renaming failed.\n");
		exit(1);
	}
}

const SrcStr& SrcStr::operator=(const char *s)
{
	if(strlen(s) >= srclen) {
		printf("Too long string.\n");
		exit(1);
	}
	int i = 0;
	while(s[i] != '\0') {
		str[i] = s[i];
		++i;
	}
	str[i] = '\0';
	return *this;
}
//class ParsedStr
ParsedStr::ParsedStr() : SrcStr()
{
	origl = new char[orglen];
	origl[0] = '\0';
	tranl = new char[trllen];
	tranl[0] = '\0';
	status = new char[stlen];
	status[0] = '\0';
	strcpy(status, "new");
	date = new char[dtlen];
	date[0] = '\0';
	const char *d = GetCurrentDate();
	ChangeDate(d);
	delete[] d;
}

ParsedStr::ParsedStr(const char *s) : ParsedStr()
{
	Parse(s);
}

ParsedStr::ParsedStr(const SrcStr& s) : ParsedStr()
{
	Parse(s.GetPtr());
}

ParsedStr::ParsedStr(const ParsedStr& s) : ParsedStr()
{
	Parse(s.GetPtr());
}

void ParsedStr::Parse(const char *s)
{
	if(!IsCorrectString(s)) {
		printf("Incorrect string:\n%s", s);
		exit(1);
	}
	strcpy(str, s);
	int i = 1;
	int j = 0;
	while(s[i] != delimiter && j < orglen) {
		origl[j++] = s[i++];
	}
	origl[j] = '\0';
	j = 0;
	++i;
	while(s[i] != delimiter && j < trllen) {
		tranl[j++] = s[i++];
	}
	tranl[j] = '\0';
	j = 0;
	++i;
	while(s[i] != delimiter && j < stlen) {
		status[j++] = s[i++];
	}
	status[i] = '\0';
	j = 0;
	++i;
	while(s[i] != '\n' && j < dtlen) {
		date[j++] = s[i++];
	}
	date[j] = '\0';
}

void ParsedStr::ChangeOriginal(const char *ns)
{
	if(strlen(ns) >= orglen) {
		printf("Too long string:\n%s", ns);
		exit(1);
	}
	int i = 0;
	while(ns[i] != '\0') {
		origl[i] = ns[i];
		++i;
	}
	origl[i] = '\0';
	RefreshSourceString();
}

void ParsedStr::ChangeTranslation(const char *ns)
{
	if(strlen(ns) >= trllen) {
		printf("Too long string:\n%s", ns);
		exit(1);
	}
	int i = 0;
	while(ns[i] != '\0') {
		tranl[i] = ns[i];
		++i;
	}
	tranl[i] = '\0';
	RefreshSourceString();
}

void ParsedStr::ChangeStatus()
{
	if(strcmp(status, st_new) == 0) {
		strcpy(status, st_rem);
		RefreshSourceString();
	}
	else {
		strcpy(status, st_new);
		RefreshSourceString();
	}
}

void ParsedStr::ChangeDate(const char *ns)
{
	if(strlen(ns) >= dtlen) {
		printf("Too long string:\n%s", ns);
		exit(1);
	}
	int i = 0;
	while(ns[i] != '\0') {
		date[i] = ns[i];
		++i;
	}
	date[i] = '\0';
	RefreshSourceString();
}

void ParsedStr::RefreshSourceString()
{
	int i = 0;
	int j = 0;
	str[j++] = delimiter;
	while(origl[i] != '\0') {
		str[j++] = origl[i++];	
	}
	str[j++] = delimiter;
	i = 0;
	while(tranl[i] != '\0') {
		str[j++] = tranl[i++];	
	}
	str[j++] = delimiter;
	i = 0;
	while(status[i] != '\0') {
		str[j++] = status[i++];	
	}
	str[j++] = delimiter;
	i = 0;
	while(date[i] != '\0') {
		str[j++] = date[i++];	
	}
	str[j++] = '\n';
	str[j++] = '\0';
}

bool ParsedStr::CmpByOriginal(const char *string) const
{
	ParsedStr ps(string);	
	return (strcmp(origl, ps.GetOriginal()) ? false : true);
}

void ParsedStr::DeleteStringFromFile(const char *destf, const char *tmpf) const
{
	DFile dest;
	DFile tmp;
	dest.OpenR(destf);
	tmp.OpenW(tmpf);
	char buf[srclen];
	bool done = false;
	while(fgets(buf, sizeof(buf), dest.GetFl())) {
		if(!done) {
			if(!CmpByOriginal(buf))
				fputs(buf, tmp.GetFl());
			else
				done = true;
		}
		else
			fputs(buf, tmp.GetFl());
	}
	dest.Close();
	tmp.Close();
	if(!done) {
		printf("The word don't exist.\n");
		exit(1);
	}
	int code;
	code = rename(tmpf, destf);
	if(code != 0) {
		printf("Renaming failed.\n");
		exit(1);
	}
}

void ParsedStr::ReplaceByOriginalInFile(const char *destf, const char *tmpf)
{
	DFile dest;
	DFile tmp;
	dest.OpenR(destf);
	tmp.OpenW(tmpf);
	char buf[srclen];
	bool done = false;
	while(fgets(buf, sizeof(buf), dest.GetFl())) {
		if(!done) {
			if(!CmpByOriginal(buf))
				fputs(buf, tmp.GetFl());
			else {
				fputs(str, tmp.GetFl());	
				strcpy(str, buf);
				Parse(str);
				done = true;
			}
		}
		else
			fputs(buf, tmp.GetFl());
	}
	dest.Close();
	tmp.Close();
	int code;
	code = rename(tmpf, destf);
	if(code != 0) {
		printf("Renaming failed.\n");
		exit(1);
	}
	if(!done) {
		printf("The word don't exist.\n");
		exit(1);
	}
}

bool ParsedStr::IsCorrectString(const char *string)
{
	if(strlen(string) > srclen-1)
		return false;
	int count = 0;
	int i = 0;
	while(string[i] != '\0') {  //count of delimiters
		if(string[i] == delimiter)
			++count;
		++i;
	}
	if(count != pcount)
		return false;
	if(string[0] != delimiter)
		return false;
	i = 1;
	count = 0;
	while(string[i++] != delimiter) { //lenght of original word
		++count;
	}
	if(count >= orglen)
		return false;	
	count = 0;
	while(string[i++] != delimiter) { //lenght of translation
		++count;
	}
	if(count >= trllen)
		return false;	
	count = 0;
	while(string[i++] != delimiter) { //lenght of status
		++count;
	}
	if(count >= stlen)
		return false;	
	count = 0;
	while(string[i++] != '\n') { //lenght of date
		++count;
	}
	if(count >= dtlen)
		return false;	
	return true;
}

char* ParsedStr::GetCurrentDate() const //????
{
	char* date = new char[dtlen];
	struct tm *tmp;
	const time_t timer = time(NULL);
	tmp = localtime(&timer);
	strftime(date, dtlen, "%d-%m-%Y", tmp);
	date[dtlen] = '\0';
	return date;
}

ParsedStr::ParsedStr(const char *ostr, const char *tstr) : ParsedStr()
{
	ChangeOriginal(ostr);
	ChangeTranslation(tstr);
}
ParsedStr::~ParsedStr()
{
	delete[] origl;
	delete[] tranl;
	delete[] status;
	delete[] date;
}

const ParsedStr& ParsedStr::operator=(const char *s)
{
	if(strlen(s) >= srclen) {
		printf("Too long string.\n");
		exit(1);
	}
	Parse(s);
	return *this;
}

char* ParsedStr::FindByOriginal(const char *namef) const
{
	DFile tmp;
	tmp.OpenR(namef);
	char buf[srclen];
	char *s = nullptr;
	while(fgets(buf, sizeof(buf), tmp.GetFl())) {
		ParsedStr ps(buf);
		if(strcmp(origl, ps.GetOriginal()) == 0) {
			s = new char[srclen];
			strcpy(s, buf);
			tmp.Close();
			return s;
		}
	}
	tmp.Close();
	s = new char[srclen];
	s[0] = '\0';
	return s;
}
