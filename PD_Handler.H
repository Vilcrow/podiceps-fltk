#ifndef PD_HANDLER_H_SENTRY
#define PD_HANDLER_H_SENTRY

#define TEST "files/test.txt"
#define TEMP "files/temp.txt"

enum { maxarg = 10 };

void help_page();
void add_word(const char* ostr, const char* tstr);
void delete_word(const char* word);
void change_original(const char* old_word, const char* new_word);
void change_translate(const char* old_word, const char* new_word);
void change_status(const char* word, const char* new_status);
void view_word();
bool find_by_original(const char* original, const char* string);
void put_new_string(const PD_Card str, const char* dest, const char* temp);

#endif
