/*
podiceps - pocket dictionary

Copyright (C) 2022 S.V.I 'Vilcrow', <vilcrow.net>
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

#ifndef DFILE_H_SENTRY
#define DFILE_H_SENTRY

#define _(STR) (STR)
#define N_(STR) (STR)

enum reqpart { rp_origl, rp_tranl, rp_st, rp_dt }; //required part

class ParsedStr {
	char *str; //source string
	char *origl; //original word
	char *tranl; //translation
	char *status;
	char *date; //date when word added
public:
	enum { orglen = 35, trllen = orglen*2, stlen = 15, dtlen = 11, pcount = 4 };
	enum { srclen = orglen+trllen+stlen+dtlen+pcount+1 };
	enum { delimiter = '|' }; //delimits the parts in source string
	ParsedStr(); //empty object
	ParsedStr(const char *s); //by the source string
	ParsedStr(const ParsedStr& s);
	ParsedStr(const char *ostr, const char *tstr); //by original and translation
	~ParsedStr();
	inline const char* SourceString() const { return str; }
 	inline const char* Original() const { return origl; }
	inline const char* Translation() const { return tranl; }
	inline const char* WStatus() const { return status; }
	inline const char* Date() const { return date; }
	void Original(const char *ns);
	void Translation(const char *ns);
	void WStatus(const char *new_status);
	void Date(const char *ns);
	const ParsedStr& operator=(const char *s);
	//dates comparison
	static const int CompareDates(const char *d1, const char *d2);
	static bool IsCorrectString(const char *s);
private:
	void Parse(const char *s); //make all parts by source string
	void RefreshSourceString(); //used after editing of any parts
	static char* GetCurrentDate(); //get current date in DD-MM-YYYY format
	//convert date to YYYY-MM-DD format for correct lexical comparison
	static void DateToYMD(char *dt);
};

struct word {
	ParsedStr ps;
	word *next;	
};

class WordList {
	int count; //current count of words in list
	word *first; //pointer to first element in the list
	word *last; //pointer to last element in the list
public:
	WordList(); //create list by dictionary file
	~WordList() { Clear(); }
	inline word* First() const { return first; }
	inline word* Last() const { return last; }
	//add word to end of list
	void Add(const char *s); //s is source string for ParsedStr
	void Add(const char *original, const char *translation,
									const char *status = 0);
	inline void Add(const ParsedStr *ps) { Add(ps->SourceString()); }
	//return pointer to element matching the pattern or 0
	word* Find(word *begin, const char *pattern, enum reqpart rp) const;
	//move element in the list to forward
	void MoveForward(word *w);
	void Clear(); //delete all elements in list but not in file
	void Reverse(); //change direction of list; don't change direction in file
	//sort list by requered part(orig/tran/st/dt)
	void Sort(enum reqpart, bool reverse = 0);
	void WriteToFile() const; //rewrite current file by content of the list
	//return pointer to ind-element if it exists
	ParsedStr* operator[](unsigned int ind); //for draw_cell function in FLTK
	void Delete(const char *original_name); //delete word from list and file
	void Amend(const char *original, const char *new_w, enum reqpart rp);
	inline int Count() const { return count; }
private:
	//swap two nearby elements and return pointer to right element
	word* Swap(word *prev, word *left);
	//deleted metods
	WordList(const WordList&) {}
	void operator=(const WordList&) {}
};

class DFile {
	static char *paths[2];
	FILE* fl;
	friend WordList::WordList();
	friend void WordList::WriteToFile() const;	
public:
	enum { pathlen = 256 };
	enum { dictionary_file = 0, backup_file };
	DFile() : fl(0) {} //file don't opened
	~DFile() { Close(); }
	inline void Close() { if(fl) fclose(fl); fl = 0; }
	inline FILE* Fl() const { return fl; }
	void OpenR(const char*); //mode "r"
	void OpenW(const char*); //mode "w"
	static void SetPaths(); //set paths for dictionary and backup files
	static void MakeBackup(); //make backup copy of current dictionary file
private:
	static char* FullPath(const char *name); //HOME + name of file
	//deleted metods
	DFile(const DFile&) {}
};


#endif
