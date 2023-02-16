/*
podiceps - pocket dictionary

Copyright (C) 2022-2023 S.V.I 'Vilcrow', <vilcrow.net>
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

char* DFile::paths[2];

void DFile::OpenR(const char* name)
{
	fl = fopen(name, "r");
	if(!fl) { //the file don't exists
		fl = fopen(name, "w"); //create empty file
		if(!fl)
			throw FileError(name, _("Couldn't open."));
		printf(_("Created new file.\n"));
	}
}

void DFile::OpenW(const char* name)
{
	fl = fopen(name, "w");
	if(!fl)
		throw FileError(name, _("Couldn't open."));
}

void DFile::SetPaths()
{
	const char *const paths_end[] = { N_(".podic.txt"), N_(".podic.txt.bak") };
	for(int i = 0; i < 2; ++i) {
		DFile::paths[i] = new char[pathlen];
		char *s = FullPath(paths_end[i]);
		strcpy(DFile::paths[i], s);
		delete[] s;
	}
}

void DFile::MakeBackup()
{
	DFile dictionary;
	DFile backup;
	dictionary.OpenR(DFile::paths[DFile::dictionary_file]);
	backup.OpenW(DFile::paths[DFile::backup_file]);
	char buf[ParsedStr::srclen];
	while(fgets(buf, sizeof(buf), dictionary.Fl())) {
		fputs(buf, backup.Fl());
	}
	dictionary.Close();
	backup.Close();
}

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
	strcpy(status, _("new"));
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
	Parse(s.SourceString());
}

void ParsedStr::Parse(const char *s)
{
	if(!IsCorrectString(s))
		throw InputError(1, s, _("Incorrect string"));
	int i = 0;
	int j = 0;
	while(s[i] != '\n' && j < srclen) {
		str[j++] = s[i++];
	}
	str[j++] = '\n';
	str[j] = '\0';
	i = 1;
	j = 0;
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
		throw InputError(2, ns, _("Too long string"));
	else if(ns == 0)
		throw InputError(6, _("New word"), _("Empty input"));
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
		throw InputError(2, ns, _("Too long string"));
	else if(ns == 0)
		throw InputError(6, _("New word"), _("Empty input"));
	int i = 0;
	while(ns[i] != '\0') {
		tranl[i] = ns[i];
		++i;
	}
	tranl[i] = '\0';
	RefreshSourceString();
}

void ParsedStr::WStatus(const char *new_status)
{
	if(new_status == 0) {
		if(strcmp(status, _("new")) == 0) {
			strcpy(status, _("remembered"));
		}
		else {
			strcpy(status, _("new"));
		}
	}
	else {
		strncpy(status, new_status, stlen);
		if(status[stlen-1] != '\0')
			status[stlen-1] = '\0';
	}
	RefreshSourceString();
}

void ParsedStr::Date(const char *ns)
{
	if(strlen(ns) >= dtlen)
		throw InputError(2, ns, _("Too long string"));
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

bool ParsedStr::IsCorrectString(const char *s)
{
	if(strlen(s) > srclen-1)
		return false;
	int count = 0;
	int i = 0;
	while(s[i] != '\0') {  //count of delimiters
		if(s[i] == delimiter)
			++count;
		++i;
	}
	if(count != pcount)
		return false;
	if(s[0] != delimiter)
		return false;
	i = 1;
	count = 0;
	while(s[i++] != delimiter) { //lenght of original word
		++count;
	}
	if(count >= orglen)
		return false;	
	count = 0;
	while(s[i++] != delimiter) { //lenght of translation
		++count;
	}
	if(count >= trllen)
		return false;	
	count = 0;
	while(s[i++] != delimiter) { //lenght of status
		++count;
	}
	if(count >= stlen)
		return false;	
	count = 0;
	while(s[i++] != '\n') { //lenght of date
		++count;
	}
	if(count >= dtlen)
		return false;	
	return true;
}

char* ParsedStr::GetCurrentDate()
{
	char* date = new char[dtlen];
	struct tm *tmp;
	const time_t timer = time(NULL);
	tmp = localtime(&timer);
	strftime(date, dtlen, "%d-%m-%Y", tmp);
	date[dtlen-1] = '\0';
	return date;
}

ParsedStr::ParsedStr(const char *ostr, const char *tstr) : ParsedStr()
{
	Original(ostr);
	if(tstr != 0)
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
		throw InputError(2, s, _("Too long string"));
	Parse(s);
	return *this;
}

const int ParsedStr::CompareDates(const char *d1, const char *d2)
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

char* DFile::FullPath(const char *name)
{
	const char *hmp = getenv(N_("HOME"));
	char fpath[DFile::pathlen];
	strncpy(fpath, hmp, pathlen-1);
	if(fpath[pathlen-1] != '\0')
		throw InputError(1, name, _("Incorrect string"));
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
		throw InputError(1, name, _("Incorrect string"));
	fpath[i] = '\0';
	char *s = new char[pathlen];
	strcpy(s, fpath);
	return s;
}

WordList::WordList()
{
	count = 0;
	first = 0;
	last = 0;
	DFile dictionary;
	dictionary.OpenR(DFile::paths[DFile::dictionary_file]);
	char buf[ParsedStr::srclen];
	while(fgets(buf, sizeof(buf), dictionary.Fl())) {
		Add(buf);
	}
	dictionary.Close();
}

void WordList::Add(const char *s)
{
	ParsedStr ps(s);
	if(Find(first, ps.Original(), rp_origl) != 0) //word exists in list
		throw InputError(5, ps.Original(), _("The word already exists"));
	if(first == 0) {
		first = new word;	
		first->ps = s;
		first->next = 0;
		last = first;
	}
	else {
		last->next = new word;
		last = last->next;
		last->ps = s;
		last->next = 0;
	}
	++count;
}

void WordList::Add(const char *original, const char *translation,
												const char *status)
{
	ParsedStr ps(original, translation);
	if(strcmp(status, "") != 0)
		ps.WStatus(status);
	Add(ps.SourceString());
}

void WordList::Clear()
{
	word *tmp = first;
	while(tmp != 0) {
		first = tmp->next;
		delete tmp;
		tmp = first;
	}
}

ParsedStr* WordList::operator[](unsigned int ind)
{
	unsigned int i = 0;
	word *tmp = first;
	while(tmp != 0) {
		if(i == ind) {
			return &(tmp->ps);
		}	
		tmp = tmp->next;
		++i;
	}
	return 0;  //ind exceeds the count of elements in list
}

word* WordList::Swap(word *prev, word *left)
{
	word *right = left->next;
	if(prev == 0) { //left is pointer to first element in list
		if(right == 0) //right don't exists
			return left;
		left->next = right->next;
		right->next = left;
		first = right;
		return left;
	}
	else {
		if(right == 0)
			return left;
		left->next = right->next;
		prev->next = right;
		right->next = left;
		if(right == last)
			last = left;
		return left;
	}
}

void WordList::Reverse()
{
	word *tmp = 0;
	word *new_first = 0; //first item in new list
	last = first;
	while(first != 0) {
		tmp = first;
		first = first->next;
		if(new_first == 0) {
			new_first = tmp;
			tmp->next = 0;
		}
		else {
			tmp->next = new_first;
			new_first = tmp;	
		}
	}
	first = new_first;
}

void WordList::Sort(enum reqpart rp, bool reverse)  //bubble sort
{
	if(first == 0) //empty list
		return;
	word *cur_last = last;
	word *prev = 0;
	word *left = 0;
	word *right = 0;
	int cmp = 0;
	while(cur_last != first) {
		left = first;
		while(left != cur_last) {	
			right = left->next;
			switch(rp) {
			case rp_origl:
				cmp = strcmp(left->ps.Original(), right->ps.Original());
				break;
			case rp_tranl:
				cmp = strcmp(left->ps.Translation(), right->ps.Translation());
				break;
			case rp_st:
				cmp = strcmp(left->ps.WStatus(), right->ps.WStatus());
				break;
			case rp_dt:
				cmp = ParsedStr::CompareDates(left->ps.Date(), right->ps.Date());
				break;
			}
			if(cmp > 0) {
				left = Swap(prev, left);	
				if(right == cur_last)
					break;
				prev = right;
				right = left->next;
			}
			else if(cmp == 0) {  //for translation, status and date
				cmp = strcmp(left->ps.Original(), right->ps.Original());
				if(cmp > 0) {
					left = Swap(prev, left);	
					if(right == cur_last)
						break;
					prev = right;
					right = left->next;
				}
				else {
					prev = left;
					left = right;
					right = right->next;
				}
			}
			else {
				prev = left;
				left = right;
				right = right->next;
			}
		}
		if(left == cur_last)
			cur_last = prev;
		prev = 0;
	}
	last = first;
	while(last->next != 0) { //return the last pointer to the end of list
		last = last->next;
	}
	if(reverse)
		Reverse();
	WriteToFile();
}

void WordList::WriteToFile() const //rewrite the dictionary file
{
	DFile dictionary;
	dictionary.OpenW(DFile::paths[DFile::dictionary_file]);
	word *tmp = first;
	while(tmp != 0) {
		fputs(tmp->ps.SourceString(), dictionary.Fl());
		tmp = tmp->next;
	}
	dictionary.Close();
}

void WordList::Delete(const char *original_name)
{
	word *tmp = first;
	word *prev = 0;
	bool done = false;
	while(tmp != 0) {
		if(strcmp(original_name, tmp->ps.Original()) == 0) {
			if(prev == 0) //delete the first element
				first = first->next;
			else if(tmp == last) { //delete the last element
				last = prev;
				last->next = 0;
			}
			else
				prev->next = tmp->next;
			delete tmp;
			--count;
			done = true;
			break;
		}
		prev = tmp;
		tmp = tmp->next;
	}
	if(!done)
		throw InputError(4, original_name, _("The word don't exists"));
	else
		WriteToFile();
}

void WordList::Amend(const char* original, const char* new_w, enum reqpart rp)
{
	if(new_w == 0)
		throw InputError(6, _("New word"), _("Empty input"));
	if(rp == rp_origl) {
		word *ptr_new = Find(first, new_w, rp_origl);
		if(ptr_new != 0)
			throw InputError(5, new_w, _("The word already exists"));
	}
	word *ptr_old = Find(first, original, rp_origl);
	if(ptr_old == 0)
		throw InputError(4, original, _("The word don't exists"));
	switch(rp) {
	case rp_origl:
		ptr_old->ps.Original(new_w);
		break;
	case rp_tranl:
		ptr_old->ps.Translation(new_w);
		break;
	case rp_st:
		ptr_old->ps.WStatus(new_w);
		break;
	case rp_dt:
		ptr_old->ps.Date(new_w);
		break;
	}
	WriteToFile();
}
//return pointer to the element in the list if it exists or zero pointer
//the search start with the begin pointer
word* WordList::Find(word *begin, const char *pattern, enum reqpart rp) const
{
	bool cmp;
	word *tmp = begin;
	while(tmp != 0) {
		switch(rp) {
		case rp_origl:
			cmp = is_match(tmp->ps.Original(), pattern);
			break;
		case rp_tranl:
			cmp = is_match(tmp->ps.Translation(), pattern);
			break;
		case rp_st:
			cmp = is_match(tmp->ps.WStatus(), pattern);
			break;
		case rp_dt:
			cmp = is_match(tmp->ps.Date(), pattern);
			break;
		}
		if(cmp)
			return tmp;
		else
			tmp = tmp->next;
	}
	return 0; //word not found
}

void WordList::MoveForward(word *w)
{
	if(w != 0) {
		if(w != first) {
			word *prev = first; //find previous element
			while(prev->next != w) {
				prev = prev->next;
			}
			prev->next = w->next;
			w->next = first;
			first = w;
			last = first;
			while(last->next != 0) { //return the last pointer back
				last = last->next;
			}
		}
	}
}
