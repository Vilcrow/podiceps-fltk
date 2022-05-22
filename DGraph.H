/*
podiceps - pocket dictionary

Copyright (C) 2022 S.V.I 'Vilcrow', <vilcrow.net>
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

struct controls { //include pointers to:
	WordList *word_list; //word list,
	Fl_Input *inpt[5]; //input widgets,
	Fl_Button *b[6]; //buttons,
	Fl_Output *msg; //field for messages,
	DTable *table; //table
};

class DTable : public Fl_Table_Row {
	int last_col; //number of last pressed column header
	bool reverse; //order of column sorting
	controls *ctrl;
protected:
	void draw_cell(TableContext context, int r, int c, int x, int y, int w, int h);
	void draw_sort_arrow(int x, int y, int w, int h);
	void sort_col(enum reqpart col, bool rev = false); //sort pressed column
public:
	DTable(int x, int y, int w, int h, const char *l);
	inline void SetCtrl(controls *c) { ctrl = c; }
	int handle(int e);
	void Refresh(); //redraw the table
	~DTable() {}
private:
	static void event_callback(Fl_Widget *w, void *user)
					{ ((DTable*)w)->event_callback2(); }
	void event_callback2();
};

enum { 
		frame = 5, //size of frame
		button_w = 80,
		button_h = 20,
		font_size = 14,
		input_w = 170,
		input_h = 20,
		rowcount = 4
	 };

void start_GUI();
void exit_cb(Fl_Widget *w, void *);
void add_cb(Fl_Widget *w, void *user);
void find_cb(Fl_Widget *w, void *user);
void amend_cb(Fl_Widget *w, void *user);
void delete_cb(Fl_Widget *w, void *user);
void clear_cb(Fl_Widget *w, void *user);

#endif
