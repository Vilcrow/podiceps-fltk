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

#ifndef DHANDLER_H_SENTRY
#define DHANDLER_H_SENTRY

#define TEST "files/test.txt"
#define TEMP "files/temp.txt"

class ParsedStr;
class Fl_Widget;

enum action { a_add, a_del, a_cor, a_show }; //what need to do
enum reqpart { rp_origl, rp_tranl, rp_st, rp_dt }; //required part

struct cl_arg {
	action act;
	reqpart rp;
	const char *arg1;	
	const char *arg2;	
};

struct ps_item {
	ParsedStr ps;
	ps_item *next;
};

void help_page();
void add_word(const char *ostr, const char *tstr);
void delete_word(const char *word);
void change_original(const char *old_word, const char *new_word);
void change_translation(const char *old_word, const char *new_word);
void change_status(const char *word, const char *ns);
void show_words(const char *pattr, const enum reqpart rp);
void print_word(const char *string);
bool is_match(const char *string, const char *pattern);
cl_arg* get_arguments(int argc, char **argv);
void handle_arguments(const cl_arg *cla);
int word_count();
ps_item* ps_list();
void sort_cb(Fl_Widget *w, void* user);
ps_item* sort_orgl(ps_item* ps);
ps_item* sort_trll(ps_item* ps);
ps_item* sort_st(ps_item* ps);
ps_item* sort_dt(ps_item* ps);
void write_to_file(ps_item *ps);
ps_item* reverse_list(ps_item *ps);

#endif
