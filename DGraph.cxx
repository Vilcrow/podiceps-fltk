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
#include <FL/fl_draw.H>
#include "DFile.H"
#include "DHandler.H"
#include "DGraph.H"

class DTable;
extern char *paths[3];

static const char *colsname[] = { "Original", "Translation", "Status", "Date" };

enum { 
		border = 5,
		button_w = 150,
		button_h = 30,
		font_size = 20,
		input_w = 170,
		input_h = 20,
		option_w = 120,
		option_h = 20,
		rowcount = 4
	 };

enum act_n { add, del, chg, shw };

struct controls {
	Fl_Input *inpt[5];
	Fl_Radio_Round_Button *rb[4];
	Fl_Box *b[5];
	Fl_Box *sbox;
	DTable *tr;
};

class DTable : public Fl_Table {
protected:
	void draw_cell(TableContext context, int r, int c, int x, int y, int w, int h);
public:
	DTable(int x, int y, int w, int h, const char *l);
	void SetSize(int nr);
	~DTable() {}
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
				break;
			case 1:
				in->maximum_size(ParsedStr::trllen-1);
				in->value(ps.Translation());
				break;
			case 2:
				in->maximum_size(ParsedStr::stlen-1);
				in->value(ps.WStatus());
				break;
			case 3:
				in->maximum_size(ParsedStr::dtlen-1);
				in->value(ps.Date());
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
	int win_w = 4*input_w + 2*border + 20;
	int win_h = 600;
	Fl_Double_Window *win = new Fl_Double_Window(win_w, win_h, "podiceps");
	controls *ctrl = new controls;
	const char *i_type[] = { "Original:", "Translation:", "Status:", "Date:", "New value:" };
	int i;
	int y = border + input_h;
	for(i = 0; i < 5; ++i) {
		ctrl->b[i] = new Fl_Box(border, y-input_h, input_w+3*border, input_h, i_type[i]);
		ctrl->inpt[i] = new Fl_Input(border, y, input_w+3*border, input_h);
		y += border + 2*input_h;
	}
	ctrl->sbox = new Fl_Box(border, y, 3*input_w, input_h);
	ctrl->inpt[0]->maximum_size(ParsedStr::orglen-1);
	ctrl->inpt[1]->maximum_size(ParsedStr::trllen-1);
	ctrl->inpt[2]->maximum_size(ParsedStr::stlen-1);
	ctrl->inpt[3]->maximum_size(ParsedStr::dtlen-1);
	ctrl->inpt[4]->maximum_size(ParsedStr::orglen-1);
	y = 2*border;
	const char *act[] = { "add", "delete", "change", "find" };
	for(i = 0; i < 4; ++i) {
		ctrl->rb[i] = new Fl_Radio_Round_Button(5*border+input_w, y, option_w, option_h, act[i]);
		y += option_h + border;
	}
	//start conditions
	ctrl->rb[0]->set();
	ctrl->inpt[2]->readonly(1);
	ctrl->inpt[3]->readonly(1);
	ctrl->inpt[4]->hide();
	ctrl->b[4]->hide();
	Fl_Button *b[2];
	const char *btn[] = { "Set", "Quit"};
	for(i =0; i < 2; ++i) {
		b[i] = new Fl_Button(5*border+input_w, y, button_w, button_h, btn[i]);
		y += border + button_h;
	}
	b[0]->callback(set_cb, (void*)ctrl);
	b[1]->callback(exit_cb, 0);
	for(i =0; i < 4; ++i) 
		ctrl->rb[i]->callback(refresh_cb, (void*)ctrl);
	//Fl_Text_Buffer *buf = new Fl_Text_Buffer;
	ctrl->tr = new DTable(border, y, 4*input_w+20, 20*input_h);
	int tsize = word_count();
	ctrl->tr->SetSize(tsize);
	ctrl->tr->align(FL_ALIGN_CENTER);
	win->end();
	//win->resizable(ctrl->tr);
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

void set_cb(Fl_Widget *w, void *user)
{
	controls *c = (controls*)user;
	int i = 0;
	while(c->rb[i]->value() == 0) {
		++i;
	}
	switch(i) {
	case add:
		add_word(c->inpt[0]->value(), c->inpt[1]->value());
		break;
	case del:
		delete_word(c->inpt[0]->value());
		break;
	case chg:
		if(strcmp(c->inpt[0]->value(), "") != 0){
			if(strcmp(c->inpt[1]->value(), "") != 0)
				change_translation(c->inpt[0]->value(), c->inpt[1]->value());
			else if(strcmp(c->inpt[2]->value(), "") != 0)
				change_status(c->inpt[0]->value(), c->inpt[2]->value());
			else if(strcmp(c->inpt[4]->value(), "") != 0)
				change_original(c->inpt[0]->value(), c->inpt[4]->value());
		}
		break;
	case shw:
		break;
	}		
}

void refresh_cb(Fl_Widget *w, void *user)
{
	controls *c = (controls*)user;
	int i = 0;
	while(c->rb[i]->value() == 0) {
		++i;
	}
	switch(i) {
	case add:
		c->inpt[0]->readonly(0);
		c->inpt[1]->readonly(0);
		c->inpt[2]->readonly(1);
		c->inpt[3]->readonly(1);
		c->inpt[4]->value("");
		c->inpt[4]->hide();
		c->b[4]->hide();
		break;	
	case del:
		c->inpt[0]->readonly(0);
		c->inpt[1]->readonly(1);
		c->inpt[2]->readonly(1);
		c->inpt[3]->readonly(1);
		c->inpt[4]->value("");
		c->inpt[4]->hide();
		c->b[4]->hide();
		break;	
	case chg:
		c->inpt[0]->readonly(0);
		c->inpt[1]->readonly(0);
		c->inpt[2]->readonly(0);
		c->inpt[3]->readonly(1);
		c->inpt[4]->value("");
		c->inpt[4]->show();
		c->b[4]->show();
		break;	
	case shw:
		c->inpt[0]->readonly(0);
		c->inpt[1]->readonly(0);
		c->inpt[2]->readonly(0);
		c->inpt[3]->readonly(0);
		c->inpt[4]->value("");
		c->inpt[4]->hide();
		c->b[4]->hide();
	}	
}
