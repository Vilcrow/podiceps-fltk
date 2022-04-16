#ifndef PD_FILE_H_SENTRY
#define PD_FILE_H_SENTRY

class DFile {
	FILE* fl;
public:
	DFile() { fl = nullptr; } //file don't opened
	~DFile() { Close(); }
	inline void Close() { if(fl) fclose(fl); fl = nullptr; }
	inline FILE* GetFl() const { return fl; }
//functions for opening the file
	void OpenR(const char*); //mode "r"
	void OpenW(const char*); //mode "w"
private:
	DFile(const DFile&) {} //no copying
	void OpenRP(const char*); //mode "r+"
	void OpenWP(const char*); //mode "w+"
	void OpenA(const char*); //mode "a"
	void OpenAP(const char*); //mode "a+"
};
//class SrcStr
//format: |original|translate|status|date
class SrcStr {
public:
	enum { srclen = 105 };
	SrcStr() { str = new char[srclen]; str[0] = '\0'; }
	SrcStr(const char *s);
	SrcStr(const SrcStr& s);
	virtual ~SrcStr() { delete[] str; }
	inline const char* GetPtr() const { return str; }
	void AddStringToFile(const char *destf, const char *tmpf);
	virtual const SrcStr& operator=(const char *s);
protected:
	char *str;
};

class ParsedStr : public SrcStr {
	char *origl;
	char *tranl;
	char *status;
	char *date;
public:
	enum { orglen = 35, trllen = 35, stlen = 15, dtlen = 15, pcount = 4 };
	enum { delimiter = '|' };
	ParsedStr();
	ParsedStr(const char *s);
	ParsedStr(const SrcStr& s);
	ParsedStr(const ParsedStr& s);
	ParsedStr(const char *ostr, const char *tstr);
	virtual ~ParsedStr();
 	inline const char* GetOriginal() const { return origl; }
	inline const char* GetTranslation() const { return tranl; }
	inline const char* GetStatus() const { return status; }
	inline const char* GetDate() const { return date; }
	void ChangeOriginal(const char *ns);
	void ChangeTranslation(const char *ns);
	void ChangeStatus();
	void ChangeDate(const char *ns);
	bool CmpByOriginal(const char *string) const;
	void DeleteStringFromFile(const char *destf, const char *tmpf) const;
	void ReplaceByOriginalInFile(const char *destf, const char *tmpf);
	virtual const ParsedStr& operator=(const char *s);
	char* FindByOriginal(const char *namef) const;
private:
	const static char st_new[]; //status "new"
	const static char st_rem[]; //status "remembered"
	void Parse(const char *s);
	void RefreshSourceString();
	char* GetCurrentDate() const;
	bool IsCorrectString(const char *string);
};

#endif
