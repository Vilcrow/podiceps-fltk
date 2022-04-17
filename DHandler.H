#ifndef PD_HANDLER_H_SENTRY
#define PD_HANDLER_H_SENTRY

#define TEST "files/test.txt"
#define TEMP "files/temp.txt"

void help_page();
void add_word(const char* ostr, const char* tstr);
void delete_word(const char* word);
void change_original(const char* old_word, const char* new_word);
void change_translation(const char* old_word, const char* new_word);
void change_status(const char* word);
void show_words(const char* pattr);
void show_word(const char* string);
bool is_match(const char* string, const char* pattern);
void arg_handling(int argc, char **argv);

#endif
