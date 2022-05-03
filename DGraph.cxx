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

#include <FL/fl_draw.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Menu_Button.H>
#include <FL/Fl_Output.H>
#include <FL/Fl_Radio_Round_Button.H>
#include <FL/Fl_Table_Row.H>
#include "DFile.H"
#include "DHandler.H"
#include "DGraph.H"

extern char *paths[3];
static const char *colsname[] = { "Original", "Translation", "Status", "Date" };
static const char *bname[] = { "@+", "@line", "Amend", "@search", "Clear", "Quit" };

class DTable : public Fl_Table_Row {
	int last_col;
	bool reverse;
	controls *ctrl;
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
};

DTable::DTable(int x, int y, int w, int h, const char *l = 0) :
	Fl_Table_Row(x, y, w, h, l)
{
	ctrl = nullptr;
	last_col = -1;
	reverse = false;
	col_header(1);
	col_resize(0);
	col_header_height(input_h);
	row_header(0);
	row_resize(0);
	int rows_t = word_count();
	rows(rows_t);
	cols(4);
	col_width_all(input_w);
	end();
	callback(event_callback, (void*)this);
}

int DTable::handle(int e)
{
	int ret = Fl_Table_Row::handle(e);
	switch(e) {
	case FL_PUSH:
		for(int r = 0; r <rows(); ++r) {
			for(int c = 0; c < cols(); ++c) {
				if(row_selected(r)) {
					DFile cur;
					char buf[ParsedStr::srclen];
					cur.OpenR(paths[0]);
					for(int i = 0; i <= r; ++i) {
						fgets(buf, sizeof(buf), cur.Fl());
					}
					ParsedStr ps(buf);
					ctrl->inpt[0]->value("");
					ctrl->inpt[1]->value("");
					ctrl->inpt[2]->value("");
					ctrl->inpt[3]->value("");
					ctrl->inpt[0]->value(ps.Original());
					ctrl->inpt[1]->value(ps.Translation());
					ctrl->inpt[2]->value(ps.WStatus());
					ctrl->inpt[3]->value(ps.Date());
					ctrl->inpt[0]->take_focus();
				}
			}
		}
		break;
	default:
		break;
	}
	return ret;
}

void DTable::event_callback2()
{
	int c = callback_col();
	TableContext context = callback_context();
	switch(context) {
	case CONTEXT_COL_HEADER:
		if(Fl::event() == FL_RELEASE && Fl::event_button() == 1) {
			if(c == last_col) {
				reverse = !reverse;
			}
			else {
				reverse = false;
			}
			sort_col(c, reverse);
			last_col = c;
		}
	default:
		return;
	}
}

void DTable::draw_cell(TableContext context, int row = 0, int col = 0,
						int x = 0, int y = 0, int w = 0, int h = 0)
{
	int index = 0;
	DFile dct;
	dct.OpenR(paths[0]);
	static char s[40];
	char buf[ParsedStr::srclen];
	char *opened;
	for(int i = 0; i <= row; ++i) {
		opened = fgets(buf, sizeof(buf), dct.Fl());
	}
	if(opened != NULL) {
		ParsedStr ps(buf);
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
				fl_draw_box(FL_THIN_UP_BOX, x, y, w, h, FL_BACKGROUND_COLOR);
				fl_font(FL_HELVETICA | FL_BOLD, 12);
				fl_color(FL_BLACK);
				sprintf(s, "%s", colsname[col]);
				fl_draw(s, x, y, w, h, FL_ALIGN_CENTER);
				if(col == last_col)
					draw_sort_arrow(x, y, w, h);
			fl_pop_clip();
			return;
		case CONTEXT_CELL: {
			switch(col) {
			case 0:
				strncpy(buf, ps.Original(), ParsedStr::orglen);
				break;
			case 1:
				strncpy(buf, ps.Translation(), ParsedStr::trllen);
				break;
			case 2:
				strncpy(buf, ps.WStatus(), ParsedStr::stlen);
				break;
			case 3:
				strncpy(buf, ps.Date(), ParsedStr::dtlen);
				break;
			}
			int selected = row_selected(row);
			fl_draw_box(FL_THIN_UP_BOX, x, y, w, h, selected ? FL_YELLOW : FL_WHITE);
			fl_push_clip(x, y, w, h);
			fl_font(FL_HELVETICA, 14);
			fl_color(FL_BLACK);
			fl_draw(buf, x, y, w, h, FL_ALIGN_LEFT);
			fl_pop_clip();
			return;
		}
		default:
			return;
		}
	}
}

void DTable::draw_sort_arrow(int x, int y, int w, int h)
{
	int xlft = x + (w - 6) - 8;
	int xctr = x + (w - 6) - 4;
	int xrit = x + (w - 6) - 0;
	int ytop = y + (h / 2) - 4;
	int ybot = y + (h / 2) + 4;
	if(reverse) {
		fl_color(FL_BLACK);
		fl_line(xrit, ytop, xctr, ybot);
		fl_color(41);
		fl_line(xlft, ytop, xrit, ytop);
		fl_line(xlft, ytop, xctr, ybot);
	}	
	else {
		fl_color(FL_WHITE);	
		fl_line(xrit, ybot, xctr, ytop);
		fl_line(xrit, ybot, xlft, ybot);
		fl_color(41);
		fl_line(xlft, ybot, xctr, ytop);
	}
}

void start_GUI()
{
	Fl::option(Fl::OPTION_ARROW_FOCUS, 0);
	int win_w = 4*input_w + 2*frame + 20;
	int win_h = 600;
	Fl_Double_Window *win = new Fl_Double_Window(win_w, win_h, "podiceps");
	int x = frame;
	int y = frame;
	controls *ctrl = new controls;
	ctrl->tr = new DTable(x, y, 4*input_w+20, 20*input_h);
	ctrl->tr->selection_color(FL_YELLOW);
	ctrl->tr->col_header(1);
	ctrl->tr->when(FL_WHEN_RELEASE);
	ctrl->tr->SetCtrl(ctrl);
	y += 21*input_h;
	for(int i = 0; i < 4; ++i) {
		ctrl->inpt[i] = new Fl_Input(x, y, input_w, input_h);		
		x += input_w;
	}
	ctrl->inpt[0]->take_focus();
	ctrl->inpt[3]->color(48);
	x = frame;
	y += input_h + frame;
	ctrl->inpt[4] = new Fl_Input(frame, y, input_w, input_h);
	x = frame + input_w / 2;
	y += input_h + frame;
	for(int i = 0; i < 6; ++i) {
		ctrl->b[i] = new Fl_Button(x, y, button_w, button_h, bname[i]);
		ctrl->b[i]->box(FL_PLASTIC_UP_BOX);
		x += frame + button_w;
	}
	ctrl->b[0]->callback(add_cb, (void*)ctrl);
	ctrl->b[0]->tooltip("Add new word\n'Enter'");
	ctrl->b[0]->shortcut(FL_Enter);
	ctrl->b[1]->callback(delete_cb, (void*)ctrl);
	ctrl->b[1]->tooltip("Delete word\n'CTRL+d'");
	ctrl->b[1]->shortcut(FL_CTRL+'d');
	ctrl->b[2]->callback(amend_cb, (void*)ctrl);
	ctrl->b[2]->tooltip("Amendment\n'CTRL+a'");
	ctrl->b[2]->shortcut(FL_CTRL+'a');
	ctrl->b[3]->callback(find_cb, (void*)ctrl);
	ctrl->b[3]->tooltip("Search\n'CTRL+s'");
	ctrl->b[3]->shortcut(FL_CTRL+'s');
	ctrl->b[4]->callback(clear_cb, (void*)ctrl);
	ctrl->b[4]->tooltip("Clear the input fields\n'CTRL+c'");
	ctrl->b[4]->shortcut(FL_CTRL+'c');
	ctrl->b[5]->callback(exit_cb, (void*)ctrl);
	ctrl->b[5]->tooltip("Exit\n'ESC'");
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

void DTable::Refresh()
{
	int t = word_count();
	rows(t);
	redraw();
	ctrl->inpt[0]->take_focus();
}

void add_cb(Fl_Widget *w, void *user)
{
	controls *c = (controls*)user;
	if(strcmp(c->inpt[0]->value(), "") != 0) {
		add_word(c->inpt[0]->value(), c->inpt[1]->value(), c->inpt[2]->value());
		c->tr->Refresh();
	}
}

void find_cb(Fl_Widget *w, void *user)
{
	controls *c = (controls*)user;
	find_pattern *p = new find_pattern;
	if(strcmp(c->inpt[0]->value(), "") != 0) {
		p->patt = c->inpt[0]->value();
		p->rp = rp_origl;
	}
	else if(strcmp(c->inpt[1]->value(), "") != 0) {
		p->patt = c->inpt[1]->value();
		p->rp = rp_tranl;
	}
	else if(strcmp(c->inpt[2]->value(), "") != 0) {
		p->patt = c->inpt[2]->value();
		p->rp = rp_st;
	}
	else if(strcmp(c->inpt[3]->value(), "") != 0) {
		p->patt = c->inpt[3]->value();
		p->rp = rp_dt;
	}
	find_words(p);
	c->tr->redraw();
	c->inpt[0]->take_focus();
}

void amend_cb(Fl_Widget *w, void *user)
{
	controls *c = (controls*)user;
	if(strcmp(c->inpt[0]->value(), "") != 0) {
		if(strcmp(c->inpt[4]->value(), "") != 0) {
			change_original(c->inpt[0]->value(), c->inpt[4]->value());
			c->inpt[0]->value(c->inpt[4]->value());
			c->inpt[4]->value("");
		}
		change_translation(c->inpt[0]->value(), c->inpt[1]->value());
		change_status(c->inpt[0]->value(), c->inpt[2]->value());
	}
	c->tr->Refresh();
}

void delete_cb(Fl_Widget *w, void *user)
{
	controls *c = (controls*)user;
	if(strcmp(c->inpt[0]->value(), "") != 0) {
		delete_word(c->inpt[0]->value());
		c->tr->Refresh();
	}
}

void clear_cb(Fl_Widget *w, void *user)
{
	controls *c = (controls*)user;
	for(int i = 0; i < 5; ++i) {
		c->inpt[i]->value("");
	}
	c->inpt[0]->take_focus();
}

void DTable::sort_col(int col, bool rev)
{
	ps_item *first = ps_list();
	switch(col) {
	case rp_origl:
		first = sort_orgl(first);
		break;
	case rp_tranl:
		first = sort_trll(first);
		break;
	case rp_st:
		first = sort_st(first);
		break;
	case rp_dt:
		first = sort_dt(first);
		break;
	}
	if(rev)
		first = reverse_list(first);	
	write_to_file(first);
	redraw();
}
