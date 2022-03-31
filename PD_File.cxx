#ifndef PD_FILE_H_SENTRY
#define PD_FILE_H_SENTRY

#include <fcntl.h>
#include <unistd.h>
#include "PD_File.H"

void PD_File::AddString(PD_Card& card)
{
	lseek(fd, 0, SEEK_END);
	write(fd, "[", 1);
	write(fd, card.GetOrlString(), card.GetOrlLen());
	write(fd, "]", 1);
	write(fd, "[", 1);
	write(fd, card.GetTrlString(), card.GetTrlLen());
	write(fd, "]", 1);
	write(fd, "\n", 1);
}

PD_String::PD_String(PD_String& f)
{
	PD_String() = f;
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
		int i;
		while(s[i] != '\0'){
			++i;
		}
		return s[i];
	}
}

int PD_String::IsInString(const char* pattern) const
{
	int i = 0;
	int j = 0;
	int start = 0;
	while(s[j] != '\0') {
		if(pattern[i] == s[j]){
			++i;
			++j;
		}
		else {
			i = 0;
			++start;	
		}
	}
	if(s[j] == '\0' && pattern[i] == '\0')
		return start;	
	else
		return -1;
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
	while(s[i] != '\0') { //find end of current string
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
	status = n;
	original = orl;
	translate = trl;
}

PD_Card::PD_Card(PD_String& orl, PD_String& trl)
{
	status = n;
	original = new PD_String();
	translate = new PD_String();
	*original = orl;
	*translate = trl;
}

PD_Card::PD_Card(char* orl, char* trl)
{
	status = n;
	original = new PD_String();
	translate = new PD_String();
	*original = orl;
	*translate = trl;	
}

#endif
