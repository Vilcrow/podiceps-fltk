class PD_Card;

class PD_File {
	int fd;
public:
	PD_File() { fd = -1; } //file don't opened
	bool OpenRO(const char *name) {
		fd = open(name, O_RDONLY);
		return (fd != -1);
	}
	bool OpenWO(const char *name) {
		fd = open(name, O_WRONLY);
		return (fd != -1);
	}
	bool OpenRW(const char *name) {
		fd = open(name, O_RDWR);
		return (fd != -1);
	}
	void Close() { if(fd != -1) close(fd); }
	~PD_File() { if(fd != -1) close(fd); }
	int GetFd() const { return fd; }
	void AddString(PD_Card&);
private:
	PD_File(const PD_File&) {} //can't copy
};

class PD_String {
	char *s;
	int len;
public:
	enum { max = 100 };
	//enum { o_bracket = "]", c_bracket = "[" };
	inline char* GetString() const { return s; }
	inline int GetLen() const { return len; }
	PD_String() { s = new char[max]; s[0] = '\0'; len = 0; }
	PD_String(PD_String&);
	PD_String(const char* str) : PD_String() { *this = str; }
	~PD_String() { if(s) delete[] s; }
	int IsInString(const char*) const;
	char& operator[](int);
	PD_String& operator=(const char*);
	PD_String& operator=(PD_String&);
	PD_String& operator+(const char*);
	PD_String& operator+(PD_String&);
	PD_String& operator-(const int); //delete last n chars from array
	PD_String& operator+=(const char*);
	PD_String& operator+=(PD_String&);
	PD_String& operator-=(const int);
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
	int status;
public:
	enum { n, r }; //n - new word, r - remembered
	PD_Card() : original(0), translate(0), status(0) {}
	PD_Card(PD_String* orl, PD_String* trl);
	PD_Card(PD_String& orl, PD_String& trl);
	PD_Card(char* orl, char* trl);
	~PD_Card() {}
	inline void SetOriginal(const char* str) { *original = str; }
	inline void SetTranslate(const char* str) { *translate = str; }
	inline void ChangeStatus() {if(status == n) status = r; else status = n; }
	inline char* GetOrlString() const { return original->GetString(); }
	inline char* GetTrlString() const { return translate->GetString(); }
	inline int GetOrlLen() const { return original->GetLen(); }
	inline int GetTrlLen() const { return translate->GetLen(); }
};
