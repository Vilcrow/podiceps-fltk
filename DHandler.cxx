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

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "DFile.H"
#include "DHandler.H"
#include "DError.H"

extern char *paths[3];

void help_page()
{
	printf("Options:\n");
	printf("-h - Show this help.\n"); 
	printf("-a [original] [translation] - Add new word.\n"); 
	printf("-d [original] - Delete word by original.\n"); 
	printf("-O [old original] [new original] - To correct the original.\n");
	printf("-T [original] [new translation]- To correct the translation.\n"); 
	printf("-S [original] [new status] - Change status.\n"); 
	printf("-o [original],\n"); 
	printf("-t [translation],\n"); 
	printf("-s [status],\n"); 
	printf("-D [date] - Show all words matching the pattern. You can use \"*\" and \"?\".\n"); 
	printf("Without arguments - To run GUI.\n"); 
}

void add_word(const char* ostr, const char* tstr, const char *status)
{
	ParsedStr ps(ostr, tstr);
	if(strcmp(status, "") != 0)
		ps.WStatus(status);
	ps.AddStringToFile();
}

void delete_word(const char* word)
{
	ParsedStr ps;
	ps.Original(word);
	ps.DeleteStringFromFile();
}

void change_original(const char* old_word, const char* new_word)
{
	ParsedStr oldp;
	oldp.Original(old_word);
	const char *olds = oldp.FindByOriginal();
	ParsedStr nw;
	nw.Original(new_word);
	const char *ns = nw.FindByOriginal();
	try {
		if(olds[0] == '\0')
			throw InputError(4, old_word, "The word don't exist");
		if(ns[0] != '\0')
			throw InputError(5, old_word, "The word already exist");
	}
	catch(...) {
		delete[] olds;
		delete[] ns;
		throw;
	}
	oldp.DeleteStringFromFile();
	ParsedStr newp(olds);
	newp.Original(new_word);
	newp.AddStringToFile();
	delete[] olds;
	delete[] ns;
}

void change_translation(const char* word, const char* new_translation)
{
	ParsedStr ps(word, new_translation);
	ps.ReplaceByOriginalInFile();
	ps.Translation(new_translation);
	ps.ReplaceByOriginalInFile();
}

void change_status(const char* word, const char *ns)
{
	ParsedStr ps;
	ps.Original(word);
	const char *s = ps.FindByOriginal();
	try {
		if(s[0] == '\0')
			throw InputError(4, word, "The word don't exist");
	}
	catch(...) {
		delete[] s;
	}
	ps.DeleteStringFromFile();
	ParsedStr nps(s);
	nps.WStatus(ns);
	nps.AddStringToFile();
	delete[] s;
}

void show_words(const char* pttr, const enum reqpart rp)
{
	DFile cur;
	cur.OpenR(paths[0]);
	char buf[ParsedStr::srclen];
	char tmp[ParsedStr::srclen];
	bool done = false;
	ParsedStr ps;
	while(fgets(buf, sizeof(buf), cur.Fl())) {
		ps = buf;
		switch(rp) {
		case rp_origl:
			strcpy(tmp, ps.Original());
			break;
		case rp_tranl:
			strcpy(tmp, ps.Translation());
			break;
		case rp_st:
			strcpy(tmp, ps.WStatus());
			break;
		case rp_dt:
			strcpy(tmp, ps.Date());
			break;
		}
		if(is_match(tmp, pttr)) {
			print_word(buf);
			done = true;
		}
	}
	if(!done)
		throw InputError(4, pttr, "The word don't exist");
}

void print_word(const char* string)
{
	ParsedStr ps(string);
	printf("%s\n", ps.Original());
	printf("%s\n", ps.Translation());
	printf("[%s] ", ps.WStatus());
	printf("[%s]\n", ps.Date());
	printf("***********************************\n");
}

bool is_match(const char* string, const char* pattern)
{
	int i;
	for(;; string++, pattern++) {
		switch(*pattern) {
			case 0:
				return *string == 0;
			case '*':
				for(i = 0; ; i++) {
					if(is_match(string+i, pattern+1))
						return true;
					if(!string[i])
						return false;
				}
			case '?':
				if(!*string)
					return false;
				break;
			default:
				if(*string != *pattern)
					return false;
		}
	}
}

cl_arg* get_arguments(int argc, char **argv)
{
	cl_arg *cla = new cl_arg;
	int opt;
	opterr = 0; //getopt doesn't print any messages
	while((opt = getopt(argc, argv, "ha:d:O:T:S:o:t:s:D:c")) != -1) {
		switch(opt) {
		case 'h':
			help_page();
			exit(0);
		case 'a':
			cla->act = a_add;
			cla->arg1 = optarg;
			cla->arg2 = argv[optind];
			break;
		case 'd':
			cla->act = a_del;
			cla->arg1 = optarg;
			break;
		case 'O':
			cla->act = a_cor;
			cla->rp = rp_origl;
			cla->arg1 = optarg;
			cla->arg2 = argv[optind];
			break;
		case 'T':
			cla->act = a_cor;
			cla->rp = rp_tranl;
			cla->arg1 = optarg;
			cla->arg2 = argv[optind];
			break;
		case 'S':
			cla->act = a_cor;
			cla->rp = rp_st;
			cla->arg1 = optarg;
			cla->arg2 = argv[optind];
			break;
		case 'o':
			cla->act = a_show;
			cla->arg1 = optarg;
			cla->rp = rp_origl;
			break;
		case 't':
			cla->act = a_show;
			cla->rp = rp_tranl;
			cla->arg1 = optarg;
			break;
		case 's':
			cla->act = a_show;
			cla->rp = rp_st;
			cla->arg1 = optarg;
			break;
		case 'D':
			cla->act = a_show;
			cla->rp = rp_dt;
			cla->arg1 = optarg;
			break;
		case 'c':
			printf("Total count: %d\n", word_count());
			exit(0);
		default:
			help_page();
			exit(1);
		}
	}
	return cla;
}

void handle_arguments(const cl_arg *cla)
{
	switch(cla->act) {
	case a_add:
		add_word(cla->arg1, cla->arg2);
		break;
	case a_del:
		delete_word(cla->arg1);
		break;
	case a_cor:
		if(cla->rp == rp_origl)
			change_original(cla->arg1, cla->arg2);
		else if(cla->rp == rp_tranl)
			change_translation(cla->arg1, cla->arg2);
		else if(cla->rp == rp_st)
			change_status(cla->arg1, cla->arg2);
		break;
	case a_show:
		show_words(cla->arg1, cla->rp);
	}
}

int word_count()
{
	DFile dct;
	dct.OpenR(paths[0]);
	int c = 0;
	char buf[ParsedStr::srclen];
	while(fgets(buf, sizeof(buf), dct.Fl())) {
		++c;
	}
	return c;
}

ps_item* ps_list()
{
	DFile cur;
	cur.OpenR(paths[0]);
	char buf[ParsedStr::srclen];
	ps_item *first = nullptr;
	ps_item *tmp = nullptr;
	while(fgets(buf, sizeof(buf), cur.Fl())) {
		if(first == nullptr) {
			first = new ps_item;
			first->next = nullptr;
			tmp = first;
		}
		else {
			tmp->next = new ps_item;
			tmp = tmp->next;
			tmp->next = nullptr;
		}
		tmp->ps = buf;
	}
	cur.Close();
	return first;
}

ps_item* sort_orgl(ps_item *ps)
{
	ps_item *of = ps;
	if(of == nullptr)
		return of;
	ps_item *tmp = nullptr;
	ps_item *nf = nullptr;
	ps_item *ntmp = nullptr;
	ps_item *ntmpp = nullptr;
	bool done = false;
	while(of != nullptr) {
		tmp = of;
		of = of->next;
		if(nf == nullptr) {
			nf = tmp;
			nf->next = nullptr;
			done = true;
		}
		else {
			ntmp = nf;
			ntmpp = nf;
		}
		while(!done) {
			if(strcmp(tmp->ps.Original(), ntmp->ps.Original()) > 0) {
				if(ntmp->next == nullptr) {
					ntmp->next = tmp;
					tmp->next = nullptr;
					done = true;
				}
				else {
					ntmpp = ntmp;
					ntmp = ntmp->next;
				}
			}
			else {
				tmp->next = ntmp;
				if(ntmpp != ntmp)
					ntmpp->next = tmp;
				if(ntmp == nf)
					nf = tmp;
				done = true;
			}
		}
		done = false;
	}
	return nf;
}

ps_item* sort_trll(ps_item *ps)
{
	ps_item *of = ps;
	if(of == nullptr)
		return of;
	ps_item *tmp = nullptr;
	ps_item *nf = nullptr;
	ps_item *ntmp = nullptr;
	ps_item *ntmpp = nullptr;
	bool done = false;
	while(of != nullptr) {
		tmp = of;
		of = of->next;
		if(nf == nullptr) {
			nf = tmp;
			nf->next = nullptr;
			done = true;
		}
		else {
			ntmp = nf;
			ntmpp = nf;
		}
		while(!done) {
			if(strcmp(tmp->ps.Translation(), ntmp->ps.Translation()) > 0) {
				if(ntmp->next == nullptr) {
					ntmp->next = tmp;
					tmp->next = nullptr;
					done = true;
				}
				else {
					ntmpp = ntmp;
					ntmp = ntmp->next;
				}
			}
			else if(strcmp(tmp->ps.Translation(), ntmp->ps.Translation()) == 0) {
				if(strcmp(tmp->ps.Original(), ntmp->ps.Original()) > 0) {
					if(ntmp->next == nullptr) {
						ntmp->next = tmp;
						tmp->next = nullptr;
						done = true;
					}
					else {
						ntmpp = ntmp;
						ntmp = ntmp->next;
					}
				}
				else {
					tmp->next = ntmp;
					if(ntmpp != ntmp)
						ntmpp->next = tmp;
					if(ntmp == nf)
						nf = tmp;
					done = true;
				}
			}
			else {
				tmp->next = ntmp;
				if(ntmpp != ntmp)
					ntmpp->next = tmp;
				if(ntmp == nf)
					nf = tmp;
				done = true;
			}
		}
		done = false;
	}
	return nf;
}

ps_item* sort_st(ps_item *ps)
{
	ps_item *of = ps;
	if(of == nullptr)
		return of;
	ps_item *tmp = nullptr;
	ps_item *nf = nullptr;
	ps_item *ntmp = nullptr;
	ps_item *ntmpp = nullptr;
	bool done = false;
	while(of != nullptr) {
		tmp = of;
		of = of->next;
		if(nf == nullptr) {
			nf = tmp;
			nf->next = nullptr;
			done = true;
		}
		else {
			ntmp = nf;
			ntmpp = nf;
		}
		while(!done) {
			if(strcmp(tmp->ps.WStatus(), ntmp->ps.WStatus()) > 0) {
				if(ntmp->next == nullptr) {
					ntmp->next = tmp;
					tmp->next = nullptr;
					done = true;
				}
				else {
					ntmpp = ntmp;
					ntmp = ntmp->next;
				}
			}
			else if(strcmp(tmp->ps.WStatus(), ntmp->ps.WStatus()) == 0) {
				if(strcmp(tmp->ps.Original(), ntmp->ps.Original()) > 0) {
					if(ntmp->next == nullptr) {
						ntmp->next = tmp;
						tmp->next = nullptr;
						done = true;
					}
					else {
						ntmpp = ntmp;
						ntmp = ntmp->next;
					}
				}
				else {
					tmp->next = ntmp;
					if(ntmpp != ntmp)
						ntmpp->next = tmp;
					if(ntmp == nf)
						nf = tmp;
					done = true;
				}
			}
			else {
				tmp->next = ntmp;
				if(ntmpp != ntmp)
					ntmpp->next = tmp;
				if(ntmp == nf)
					nf = tmp;
				done = true;
			}
		}
		done = false;
	}
	return nf;
}

ps_item* sort_dt(ps_item *ps)
{
	ps_item *of = ps;
	if(of == nullptr)
		return of;
	ps_item *tmp = nullptr;
	ps_item *nf = nullptr;
	ps_item *ntmp = nullptr;
	ps_item *ntmpp = nullptr;
	bool done = false;
	while(of != nullptr) {
		tmp = of;
		of = of->next;
		if(nf == nullptr) {
			nf = tmp;
			nf->next = nullptr;
			done = true;
		}
		else {
			ntmp = nf;
			ntmpp = nf;
		}
		while(!done) {
			if(ParsedStr::CmpDates(tmp->ps.Date(), ntmp->ps.Date()) > 0) {
				if(ntmp->next == nullptr) {
					ntmp->next = tmp;
					tmp->next = nullptr;
					done = true;
				}
				else {
					ntmpp = ntmp;
					ntmp = ntmp->next;
				}
			}
			else if(ParsedStr::CmpDates(tmp->ps.Date(), ntmp->ps.Date())  == 0) {
				if(strcmp(tmp->ps.Original(), ntmp->ps.Original()) > 0) {
					if(ntmp->next == nullptr) {
						ntmp->next = tmp;
						tmp->next = nullptr;
						done = true;
					}
					else {
						ntmpp = ntmp;
						ntmp = ntmp->next;
					}
				}
				else {
					tmp->next = ntmp;
					if(ntmpp != ntmp)
						ntmpp->next = tmp;
					if(ntmp == nf)
						nf = tmp;
					done = true;
				}
			}
			else {
				tmp->next = ntmp;
				if(ntmpp != ntmp)
					ntmpp->next = tmp;
				if(ntmp == nf)
					nf = tmp;
				done = true;
			}
		}
		done = false;
	}
	return nf;
}

void write_to_file(ps_item *ps)
{
	DFile cur;
	cur.OpenW(paths[0]);
	ps_item *tmp = ps;
	while(ps != nullptr) {
		fputs(ps->ps.GetSrcStr(), cur.Fl());
		tmp = ps;
		ps = ps->next;
		delete tmp;
	}
	cur.Close();
}

ps_item* reverse_list(ps_item *ps)
{
	ps_item *tmp = nullptr;
	ps_item *nf = nullptr; //first item in new list
	while(ps != nullptr) {
		tmp = ps;
		ps = ps->next;
		if(nf == nullptr) {
			nf = tmp;
			tmp->next = nullptr;
		}
		else {
			tmp->next = nf;
			nf = tmp;	
		}
	}
	return nf;
}

void find_words(find_pattern *p)
{
	ps_item *first = ps_list();
	ps_item *tmp = first;
	ps_item *tmp_prev = tmp;
	bool cmp;
	bool done = false;
	while(tmp != nullptr) {
		switch(p->rp) {
		case rp_origl:
			cmp = is_match(tmp->ps.Original(), p->patt);
			break;
		case rp_tranl:
			cmp = is_match(tmp->ps.Translation(), p->patt);
			break;
		case rp_st:
			cmp = is_match(tmp->ps.WStatus(), p->patt);
			break;
		case rp_dt:
			cmp = is_match(tmp->ps.Date(), p->patt);
			break;
		}
		if(cmp) {
			if(tmp != first) {
				tmp_prev->next = tmp->next;
				tmp->next = first;
				first = tmp;
				tmp = tmp_prev->next;
			}
			else
				tmp = tmp->next;
			done = true;
		}
		else {
			tmp_prev = tmp;
			tmp = tmp->next;
		}
	}
	write_to_file(first);
	if(!done)
		throw InputError(4, p->patt, "The word don't exist");
}
