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
#include <time.h>
#include "DFile.H"
#include "DHandler.H"
#include "DError.H"

char* paths[3];

//class DFile
void DFile::OpenR(const char* name)
{
	fl = fopen(name, "r");
	if(!fl) {
		fl = fopen(name, "w");
		if(!fl)
			throw FileError(name, "Couldn't open.");
		printf("Created new file.\n");
	}
}

void DFile::OpenW(const char* name)
{
	fl = fopen(name, "w");
	if(!fl)
		throw FileError(name, "Couldn't open.");
}

void DFile::OpenA(const char* name)
{
	fl = fopen(name, "a");
	if(!fl)
		throw FileError(name, "Couldn't open.");
}

//class ParsedStr
ParsedStr::ParsedStr()
{
	str = new char[srclen];
	str[0] = '\0';
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
	Date(d);
	delete[] d;
}

ParsedStr::ParsedStr(const char *s) : ParsedStr()
{
	Parse(s);
}

ParsedStr::ParsedStr(const ParsedStr& s) : ParsedStr()
{
	Parse(s.GetSrcStr());
}

void ParsedStr::Parse(const char *s)
{
	if(!IsCorrectString(s))
		throw InputError(1, s, "Incorrect string");
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
	status[j] = '\0';
	j = 0;
	++i;
	while(s[i] != '\n' && j < dtlen) {
		date[j++] = s[i++];
	}
	date[j] = '\0';
}

void ParsedStr::Original(const char *ns)
{
	if(strlen(ns) >= orglen)
		throw InputError(2, ns, "Too long string");
	int i = 0;
	while(ns[i] != '\0') {
		origl[i] = ns[i];
		++i;
	}
	origl[i] = '\0';
	RefreshSourceString();
}

void ParsedStr::Translation(const char *ns)
{
	if(strlen(ns) >= trllen)
		throw InputError(2, ns, "Too long string");
	int i = 0;
	while(ns[i] != '\0') {
		tranl[i] = ns[i];
		++i;
	}
	tranl[i] = '\0';
	RefreshSourceString();
}

void ParsedStr::WStatus(const char *ns)
{
	if(ns == nullptr) {
		if(strcmp(status, "new") == 0) {
			strcpy(status, "remembered");
			RefreshSourceString();
		}
		else {
			strcpy(status, "new");
			RefreshSourceString();
		}
	}
	else {
		strncpy(status, ns, stlen);
		if(status[stlen-1] != '\0')
			status[stlen-1] = '\0';
		RefreshSourceString();
	}
}

void ParsedStr::Date(const char *ns)
{
	if(strlen(ns) >= dtlen)
		throw InputError(2, ns, "Too long string");
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
	return (strcmp(origl, ps.Original()) ? false : true);
}

void ParsedStr::AddStringToFile() const //exception handling, need fix
{
	if(!IsCorrectString(GetSrcStr()))
		throw InputError(1, GetSrcStr(), "Incorrect string");
	int cmp;
	char buf[srclen];
	bool is = false; //word already exists in file?
	DFile dest;
	dest.OpenR(paths[0]);
	while(fgets(buf, sizeof(buf), dest.Fl())) {
		ParsedStr ps(buf);
		cmp = strcmp(ps.Original(), Original());
		if(cmp == 0) {
			is = true;
			break;
		}
	}
	dest.Close();
	if(!is) {
		dest.OpenA(paths[0]);
		fputs(str, dest.Fl());
		dest.Close();
	}
	else
		throw InputError(5, Original(), "The word already exists");
}

void ParsedStr::DeleteStringFromFile() const
{
	DFile dest;
	DFile tmp;
	dest.OpenR(paths[0]);
	tmp.OpenW(paths[1]);
	char buf[srclen];
	bool done = false;
	while(fgets(buf, sizeof(buf), dest.Fl())) {
		if(!done) {
			if(!CmpByOriginal(buf))
				fputs(buf, tmp.Fl());
			else
				done = true;
		}
		else
			fputs(buf, tmp.Fl());
	}
	dest.Close();
	tmp.Close();
	if(!done)
		throw InputError(4, Original(), "The word don't exist");
	int code;
	code = rename(paths[1], paths[0]);
	if(code != 0)
		throw FileError(paths[1], "Renaming failed");
}

void ParsedStr::ReplaceByOriginalInFile()
{
	DFile dest;
	DFile tmp;
	dest.OpenR(paths[0]);
	tmp.OpenW(paths[1]);
	char buf[srclen];
	bool done = false;
	while(fgets(buf, sizeof(buf), dest.Fl())) {
		if(!done) {
			if(!CmpByOriginal(buf))
				fputs(buf, tmp.Fl());
			else {
				fputs(str, tmp.Fl());	
				strcpy(str, buf);
				Parse(str);
				done = true;
			}
		}
		else
			fputs(buf, tmp.Fl());
	}
	dest.Close();
	tmp.Close();
	int code;
	code = rename(paths[1], paths[0]);
	if(code != 0)
		throw FileError(paths[1], "Renaming failed");
	if(!done)
		throw InputError(4, Original(), "The word don't exist");
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
	Original(ostr);
	Translation(tstr);
}
ParsedStr::~ParsedStr()
{
	delete[] str;
	delete[] origl;
	delete[] tranl;
	delete[] status;
	delete[] date;
}

const ParsedStr& ParsedStr::operator=(const char *s)
{
	if(strlen(s) >= srclen)
		throw InputError(2, s, "Too long string");
	Parse(s);
	return *this;
}

char* ParsedStr::FindByOriginal() const
{
	DFile tmp;
	tmp.OpenR(paths[0]);
	char buf[srclen];
	char *s = nullptr;
	while(fgets(buf, sizeof(buf), tmp.Fl())) {
		ParsedStr ps(buf);
		if(strcmp(origl, ps.Original()) == 0) {
			s = new char[srclen];
			strcpy(s, buf);
			tmp.Close();
			return s;
		}
	}
	tmp.Close();
	s = new char[srclen];
	s[0] = '\0'; //return empty string
	return s;
}

void ParsedStr::SetPaths()
{
	const char *const paths_end[] = { ".podic.txt", ".podictmp.txt", ".podic.txt.bak" };
	char *s;
	for(int i = 0; i < pathcnt; ++i) {
		paths[i] = new char[pathlen];
		s = FullPath(paths_end[i]);
		strcpy(paths[i], s);
		delete[] s;
	}
}

const int ParsedStr::CmpDates(const char *d1, const char *d2)
{
	char date1[dtlen];
	strncpy(date1, d1, dtlen);
	if(date1[dtlen-1] != '\0')
		date1[dtlen-1] = '\0';
	DateToYMD(date1);
	char date2[dtlen];
	strncpy(date2, d2, dtlen);
	if(date2[dtlen-1] != '\0')
		date2[dtlen-1] = '\0';
	DateToYMD(date2);
	return strcmp(date1, date2);
}

void ParsedStr::DateToYMD(char *dt)
{
	char ndate[dtlen];
	int i = 0;
	int dash = 0;
	while(dash < 2) {
		if(dt[i++] == '-')
			++dash;
	}
	int j = 0;
	while(dt[i] != '\0') {
		ndate[j++] = dt[i++];
	}
	ndate[j++] = '-';
	i = 0;
	dash = 0;
	while(dash < 1) {
		if(dt[i++] == '-')
			++dash;
	}
	while(dt[i] != '-') {
		ndate[j++] = dt[i++];
	}
	ndate[j++] = '-';
	i = 0;
	while(dt[i] != '-') {
		ndate[j++] = dt[i++];
	}
	ndate[j++] = '\0';
	strcpy(dt, ndate);
}

char* ParsedStr::FullPath(const char *name)
{
	const char *hmp = getenv("HOME");
	char fpath[pathlen];
	strncpy(fpath, hmp, pathlen-1);
	if(fpath[pathlen-1] != '\0')
		throw InputError(1, name, "Incorrect string");
	int i = 0;
	while(fpath[i] != '\0' && i < pathlen-1) {
		++i;
	}
	fpath[i++] = '/';
	int j = 0;
	while(name[j] != '\0' && i < pathlen-1) {
		fpath[i++] = name[j++];
	}
	if(name[j] != '\0')
		throw InputError(1, name, "Incorrect string");
	fpath[i] = '\0';
	char *s = new char[pathlen];
	strcpy(s, fpath);
	return s;
}

void make_bak_file()
{
	DFile dic;
	DFile bak;
	dic.OpenR(paths[0]);
	bak.OpenW(paths[2]);
	char buf[ParsedStr::srclen];
	while(fgets(buf, sizeof(buf), dic.Fl())) {
		fputs(buf, bak.Fl());
	}
}
