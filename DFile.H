#ifndef PD_FILE_H_SENTRY
#define PD_FILE_H_SENTRY

class DFile {
	FILE* fl;
public:
	DFile() { fl = nullptr; } //file don't opened
	~DFile() { Close(); }
	inline void Close() { if(fl) fclose(fl); fl = nullptr; }
	inline FILE* GetFl() const { return fl; }
	void OpenR(const char*); //mode "r"
	void OpenW(const char*); //mode "w"
private:
	DFile(const DFile&) {} //no copying
};
//class ParsedStr
//format: |original|translate|status|date
class ParsedStr {
	char *str;
	char *origl;
	char *tranl;
	char *status;
	char *date;
	friend void show_words(const char *pttr);
public:
	enum { srclen = 105, pathlen = 256, pathcnt = 3 };
	enum { orglen = 35, trllen = 35, stlen = 15, dtlen = 15, pcount = 4 };
	enum { delimiter = '|' };
	ParsedStr();
	ParsedStr(const char *s);
	ParsedStr(const ParsedStr& s);
	ParsedStr(const char *ostr, const char *tstr);
	~ParsedStr();
	inline const char* GetSrcStr() const { return str; }
 	inline const char* GetOriginal() const { return origl; }
	inline const char* GetTranslation() const { return tranl; }
	inline const char* GetStatus() const { return status; }
	inline const char* GetDate() const { return date; }
	void ChangeOriginal(const char *ns);
	void ChangeTranslation(const char *ns);
	void ChangeStatus();
	void ChangeDate(const char *ns);
	bool CmpByOriginal(const char *string) const;
	void AddStringToFile() const;
	void DeleteStringFromFile() const;
	void ReplaceByOriginalInFile();
	const ParsedStr& operator=(const char *s);
	char* FindByOriginal() const;
	static void SetPaths();
private:
	void Parse(const char *s);
	void RefreshSourceString();
	char* GetCurrentDate() const;
	bool IsCorrectString(const char *string);
	static char* FullPath(const char *name);
};

#endif
