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

#ifndef DGRAPH_H_SENTRY
#define DGRAPH_H_SENTRY

#include <FL/Fl_Table_Row.H>

class DTable;

struct controls {
	Fl_Input *inpt[5];
	Fl_Button *b[6];
	DTable *tr;
	Fl_Output *msg;
};

class StringsList {
	struct string {
		ParsedStr ps;
		string *next;	
	};
	string *first;
	string *last;
public:
	StringsList() : first(0), last(0) {}
	~StringsList() { Clear(); }
	void Add(const char *s);
	void Clear();
	ParsedStr* operator[](unsigned int ind);
private:
	StringsList(const StringsList&) {}
	void operator=(const StringsList&) {}
};

class DTable : public Fl_Table_Row {
	int last_col;
	bool reverse;
	controls *ctrl;
	StringsList sl;
protected:
	void draw_cell(TableContext context, int r, int c, int x, int y, int w, int h);
	void draw_sort_arrow(int x, int y, int w, int h);
	void sort_col(int col, bool rev = false);
public:
	DTable(int x, int y, int w, int h, const char *l);
	inline void SetCtrl(controls *c) { ctrl = c; }
	int handle(int e);
	void Refresh();
	~DTable() {}
private:
	static void event_callback(Fl_Widget *w, void *user)
					{ ((DTable*)w)->event_callback2(); }
	void event_callback2();
	void UpdateStringsList();
};

enum { 
		frame = 5,
		button_w = 80,
		button_h = 20,
		font_size = 20,
		input_w = 170,
		input_h = 20,
		dash_w = 35,
		dash_h = 20,
		rowcount = 4
	 };

void start_GUI();
void exit_cb(Fl_Widget *w, void *);
void refresh_table(controls *c);
void add_cb(Fl_Widget *w, void *user);
void find_cb(Fl_Widget *w, void *user);
void amend_cb(Fl_Widget *w, void *user);
void delete_cb(Fl_Widget *w, void *user);
void clear_cb(Fl_Widget *w, void *user);

#endif
