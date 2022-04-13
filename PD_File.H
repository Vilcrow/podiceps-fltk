#ifndef PD_FILE_H_SENTRY
#define PD_FILE_H_SENTRY

enum { size_date = 11 };
enum { max_string = 100 };

class PD_File {
	FILE* fl;
public:
	PD_File() { fl = nullptr; } //file don't opened
	~PD_File() { Close(); }
	void OpenR(const char*); //mode "r"
	void OpenRP(const char*); //mode "r+"
	void OpenW(const char*); //mode "w"
	void OpenWP(const char*); //mode "w+"
	void OpenA(const char*); //mode "a"
	void OpenAP(const char*); //mode "a+"
	inline void Close() { if(fl) fclose(fl); fl = nullptr; }
	inline FILE* GetFl() const { return fl; }
private:
	PD_File(const PD_File&) {} //no copying
};

class PD_String {
	char *s;
	int len;
public:
	enum { max = 100 };
	PD_String() { s = new char[max]; s[0] = '\0'; len = 0; }
	PD_String(PD_String&); //copying
	PD_String(const char* str) : PD_String() { *this = str; }
	~PD_String() { delete[] s; }
	char& operator[](int);
	PD_String& operator=(const char*);
	PD_String& operator=(PD_String&);
	PD_String& operator+(const char*);
	PD_String& operator+(PD_String&);
	PD_String& operator-(const int); //delete last n characters from array
	PD_String& operator+=(const char*);
	PD_String& operator+=(PD_String&);
	PD_String& operator-=(const int);
	inline const char* GetString() const { return s; }
	inline const int GetLen() const { return len; }
private:
	void operator+=(const int) {}
	void operator-(const char*) {}
	void operator-(const PD_String&) {}
	void operator-=(const char*) {}
	void operator-=(const PD_String&) {}
};

class PD_Card {
	PD_String* original; //word in original language
	PD_String* translate;
	int st; //status of word
	char date[size_date];
public:
	enum { n, r }; //n - new word, r - remembered
	PD_Card();
	PD_Card(PD_String* orl, PD_String* trl);
	PD_Card(PD_String& orl, PD_String& trl);
	PD_Card(const char* orl, const char* trl);
	PD_Card(const char*); //constructor from full string
	~PD_Card() {} //need fix
	void SetOriginal(const char* );
	void SetTranslate(const char* );
	void SetDate(const char* str);
	inline void ChangeStatus() {if(st == n) st = r; else st = n; }
	inline const char* GetOrlString() const { return original->GetString(); }
	inline const char* GetTrlString() const { return translate->GetString(); }
	inline int GetOrlLen() const { return original->GetLen(); }
	inline int GetTrlLen() const { return translate->GetLen(); }
	inline int GetStatus() const { return st; }
	inline const char* GetDate() const { return date; } //???
	void PrintToFile(FILE*) const; //print string to specified stream
	char* MakeString() const;
private:
	char* GetCurrentDate() const; //format DD-MM-YYYY
};

//(original)|translate|{status}[date]
class PD_FullString : public PD_String {
public:
};

#endif
