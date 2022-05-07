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

class DTable;

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

struct controls {
	Fl_Input *inpt[5];
	Fl_Button *b[6];
	DTable *tr;
	Fl_Output *msg;
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
