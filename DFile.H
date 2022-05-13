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

#ifndef DFILE_H_SENTRY
#define DFILE_H_SENTRY

#define _(STR) (STR)
#define N_(STR) (STR)

class DFile {
	FILE* fl;
public:
	DFile() { fl = nullptr; } //file don't opened
	~DFile() { Close(); }
	inline void Close() { if(fl) fclose(fl); fl = nullptr; }
	inline FILE* Fl() const { return fl; }
	void OpenR(const char*); //mode "r"
	void OpenW(const char*); //mode "w"
	void OpenA(const char*); //mode "a"
private:
	DFile(const DFile&) {} //no copying
};

class ParsedStr {
	char *str;
	char *origl;
	char *tranl;
	char *status;
	char *date;
public:
	enum { orglen = 35, trllen = orglen*2, stlen = 15, dtlen = 11, pcount = 4 };
	enum { srclen = orglen+trllen+stlen+dtlen+pcount+1, pathlen = 256, pathcnt = 3 };
	enum { delimiter = '|' };
	ParsedStr();
	ParsedStr(const char *s);
	ParsedStr(const ParsedStr& s);
	ParsedStr(const char *ostr, const char *tstr);
	~ParsedStr();
	inline const char* SourceString() const { return str; }
 	inline const char* Original() const { return origl; }
	inline const char* Translation() const { return tranl; }
	inline const char* WStatus() const { return status; }
	inline const char* Date() const { return date; }
	void Original(const char *ns);
	void Translation(const char *ns);
	void WStatus(const char *ns);
	void Date(const char *ns);
	bool CmpByOriginal(const char *string) const;
	void AddStringToFile() const;
	void DeleteStringFromFile() const;
	void ReplaceByOriginalInFile();
	const ParsedStr& operator=(const char *s);
	char* FindByOriginal() const;
	static void SetPaths();
	static const int CmpDates(const char *d1, const char *d2);
	static bool IsCorrectString(const char *s);
private:
	void Parse(const char *s);
	void RefreshSourceString();
	static char* GetCurrentDate();
	static char* FullPath(const char *name);
	//convert date to YYYY-MM-DD format correct lexical comparison
	static void DateToYMD(char *dt);
};

void make_bak_file(); //make backup copy of current dictonary file

#endif
