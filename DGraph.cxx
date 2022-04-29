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

#include <FL/Fl.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Radio_Round_Button.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Table.H>
#include <FL/Fl_Table_Row.H>
#include <FL/Fl_Menu_Button.H>
#include <FL/fl_draw.H>
#include "DFile.H"
#include "DHandler.H"
#include "DGraph.H"

extern char *paths[3];
static const char *colsname[] = { "Original", "Translation", "Status", "Date" };
bool reverse = false; //temporary global variable

class DTable : public Fl_Table {
protected:
	void draw_cell(TableContext context, int r, int c, int x, int y, int w, int h);
public:
	DTable(int x, int y, int w, int h, const char *l);
	void SetSize(int nr);
	~DTable() {}
private:
	static void event_callback(Fl_Widget *w, void *user);
	void event_callback2();
};

DTable::DTable(int x, int y, int w, int h, const char *l = 0) :
	Fl_Table(x, y, w, h, l)
{
	col_header(1);
	col_resize(0);
	col_header_height(input_h);
	row_header(0);
	row_resize(0);
	end();
	callback(event_callback, (void*)this);
}

void DTable::event_callback(Fl_Widget *w, void *user)
{
	DTable *t = (DTable*)user;
	t->event_callback2();
}

void DTable::event_callback2()
{
	int c = callback_col();
	const int *pc = &c;
	int tsize = word_count();
	TableContext context = callback_context();
	switch(context) {
	case CONTEXT_COL_HEADER:
		if(Fl::event() == FL_RELEASE && Fl::event_button() == 1) {
			switch(c) {
			case rp_origl:
				sort_cb(this, (void*)pc);
				SetSize(tsize);
				reverse = !reverse;
				break;
			case rp_tranl:
				sort_cb(this, (void*)pc);
				SetSize(tsize);
				reverse = !reverse;
				break;
			case rp_st:
				sort_cb(this, (void*)pc);
				SetSize(tsize);
				reverse = !reverse;
				break;
			case rp_dt:
				sort_cb(this, (void*)pc);
				SetSize(tsize);
				reverse = !reverse;
				break;
			}
		}
	default:
		return;
	}
}

void DTable::draw_cell(TableContext context, int row = 0, int col = 0,
						int x = 0, int y = 0, int w = 0, int h = 0)
{
	int index = 0;
	static char s[40];
	switch(context) {
	case CONTEXT_STARTPAGE:
		fl_font(FL_HELVETICA, 12);
		break;
	case CONTEXT_RC_RESIZE:
		for(int r = 0; r < rows(); ++r) {
			for(int c = 0; c < cols(); ++c) {
				if(index >= children())
					break;
				find_cell(CONTEXT_TABLE, r, c, x, y, w, h);
				child(index++)->resize(x, y, w, h);
			}
		}
		init_sizes();
		return;
	case CONTEXT_COL_HEADER:
		fl_push_clip(x, y, w, h);
		sprintf(s, "%s", colsname[col]);
		fl_draw_box(FL_THIN_UP_BOX, x, y, w, h, col_header_color());
		fl_color(FL_BLACK);
		fl_draw(s, x, y, w, h, FL_ALIGN_CENTER);
		fl_pop_clip();
		return;
	case CONTEXT_CELL:
		fl_push_clip(x, y, w, h);
		fl_pop_clip();
		return;
	default:
		return;
	}
}

void DTable::SetSize(int nr)
{
	clear();
	rows(nr);
	cols(4);
	DFile dct;
	dct.OpenR(paths[0]);
	char buf[ParsedStr::srclen];
	begin();
	for(int r = 0; r < nr; ++r) {
		fgets(buf, sizeof(buf), dct.GetFl());
		ParsedStr ps(buf);
		for(int c = 0; c < 4; ++c) {
			int x, y, w, h;
			find_cell(CONTEXT_TABLE, r, c, x, y, w, h);
			Fl_Input *in = new Fl_Input(x, y, w, h);
			switch(c) {
			case 0:
				in->maximum_size(ParsedStr::orglen-1);
				in->value(ps.Original());
				in->readonly(1);
				break;
			case 1:
				in->maximum_size(ParsedStr::trllen-1);
				in->value(ps.Translation());
				in->readonly(1);
				break;
			case 2:
				in->maximum_size(ParsedStr::stlen-1);
				in->value(ps.WStatus());
				in->readonly(1);
				break;
			case 3:
				in->maximum_size(ParsedStr::dtlen-1);
				in->value(ps.Date());
				in->readonly(1);
				break;
			}
		}
	}
	col_width_all(input_w);
	end();
	dct.Close();
}

void start_GUI()
{
	int win_w = 4*input_w + 2*frame + 20;
	int win_h = 600;
	Fl_Double_Window *win = new Fl_Double_Window(win_w, win_h, "podiceps");
	Fl_Menu_Button *menu = new Fl_Menu_Button(0, 20, 640, 480);
	menu->type(Fl_Menu_Button::POPUP3);
	menu->add("Change", 0, exit_cb, (void*)"Change");
	menu->add("Delete", 0, exit_cb, (void*)"Delete");
	controls *ctrl = new controls;
	const char *i_name[] = { "Original:", "Translation:" };
	int i;
	int x = frame;
	int y = frame;
	for(i = 0; i < 2; ++i) {
		ctrl->inptname[i] = new Fl_Box(x, y, input_w, input_h, i_name[i]);
		y += input_h;
		ctrl->inpt[i] = new Fl_Input(x, y, input_w+3*frame, input_h);
		x += frame + input_w + dash_w;
		y = frame;
	}
	x = 3*frame + input_w;
	y = frame + input_h;
	ctrl->inptname[++i] = new Fl_Box(x, y, dash_w, dash_h, "-");
	ctrl->inptname[i]->align(FL_ALIGN_CENTER);
	ctrl->inpt[0]->maximum_size(ParsedStr::orglen-1);
	ctrl->inpt[1]->maximum_size(ParsedStr::trllen-1);
	Fl_Button *b[2];
	x = 8*frame + dash_w + 2*input_w;
	y = input_h + frame;
	const char *btn[] = { "Add", "Quit"};
	for(i =0; i < 2; ++i) {
		b[i] = new Fl_Button(x, y, button_w, button_h, btn[i]);
		x += frame + button_w;
	}
	b[0]->callback(add_cb, (void*)ctrl);
	b[1]->callback(exit_cb, 0);
	y = 2*input_h + 2*frame;
	ctrl->tr = new DTable(frame, y, 4*input_w+20, 20*input_h);
	int tsize = word_count();
	ctrl->tr->SetSize(tsize);
	ctrl->tr->align(FL_ALIGN_CENTER);
	ctrl->tr->selection_color(FL_YELLOW);
	ctrl->tr->col_header(1);
	ctrl->tr->when(FL_WHEN_RELEASE);
	win->end();
	win->show();
	Fl::run();
}

void exit_cb(Fl_Widget *w, void *)
{
	Fl_Widget *p;
	do {
		p = w->parent();
		if(p)
			w = p;
	} while(p);
	w->hide();
}

void add_cb(Fl_Widget *w, void *user)
{
	controls *c = (controls*)user;
	if(strcmp(c->inpt[0]->value(), "") != 0) {
		add_word(c->inpt[0]->value(), c->inpt[1]->value());
		refresh_table(c);
	}
}

void refresh_table(controls *c)
{
	int s = word_count();
	c->tr->SetSize(s);
}

