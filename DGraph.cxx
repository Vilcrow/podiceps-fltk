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

#include <FL/fl_draw.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Menu_Button.H>
#include <FL/Fl_Output.H>
#include <FL/Fl_Radio_Round_Button.H>
#include <FL/Fl_Table_Row.H>
#include "DFile.H"
#include "DHandler.H"
#include "DGraph.H"
#include "DError.H"

static const char *colsname[] = { _("Original"), _("Translation"),
								  _("Status"), _("Date") };

static const char *bname[] = { N_("@+"), N_("@line"), _("Amend"),
							   N_("@search"), _("Clear"), _("Quit") };

DTable::DTable(int x, int y, int w, int h, const char *l = 0) :
	Fl_Table_Row(x, y, w, h, l)
{
	ctrl = 0;
	last_col = -1;
	reverse = false;
	cols(4);
	col_header(1);
	col_resize(1);
	col_header_height(input_h);
	col_width_all(input_w);
	row_header(0);
	row_resize(0);
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
					ParsedStr *ps = (*ctrl->word_list)[r];
					ctrl->inpt[0]->value("");
					ctrl->inpt[1]->value("");
					ctrl->inpt[2]->value("");
					ctrl->inpt[3]->value("");
					ctrl->inpt[0]->value(ps->Original());
					ctrl->inpt[1]->value(ps->Translation());
					ctrl->inpt[2]->value(ps->WStatus());
					ctrl->inpt[3]->value(ps->Date());
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
			sort_col((enum reqpart)c, reverse);
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
	static char s[ParsedStr::orglen];
	static char buf[ParsedStr::srclen];
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
			strncpy(buf, (*ctrl->word_list)[row]->Original(), ParsedStr::orglen);
			break;
		case 1:
			strncpy(buf, (*ctrl->word_list)[row]->Translation(), ParsedStr::trllen);
			break;
		case 2:
			strncpy(buf,(*ctrl->word_list)[row]->WStatus(), ParsedStr::stlen);
			break;
		case 3:
			strncpy(buf, (*ctrl->word_list)[row]->Date(), ParsedStr::dtlen);
			break;
		}
		int selected = row_selected(row);
		fl_draw_box(FL_THIN_UP_BOX, x, y, w, h, selected ? FL_YELLOW : FL_WHITE);
		fl_push_clip(x, y, w, h);
		fl_font(FL_HELVETICA, font_size);
		fl_color(FL_BLACK);
		fl_draw(buf, x, y, w, h, FL_ALIGN_LEFT);
		fl_pop_clip();
		return;
	}
	default:
		return;
	}
	if(col_width(0) >= ctrl->table->w() || col_width(1) >= ctrl->table->w() ||
										col_width(2) >= ctrl->table->w())
		col_width_all(ctrl->table->w()/4);
	//table fills all available field of window by expanding the last column
	int lc_w = ctrl->table->w() - col_width(0) - col_width(1) - col_width(2)
										- Fl::scrollbar_size()-frame;
	col_width(3, lc_w);
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
	int win_h = 5*frame + 26*input_h;
	Fl_Double_Window *win = new Fl_Double_Window(win_w, win_h, N_("podiceps"));
	int x = frame;
	int y = frame;
	controls *ctrl = new controls;
	ctrl->word_list = new WordList;
	ctrl->table = new DTable(x, y, 4*input_w+20, 20*input_h);
	ctrl->table->rows(ctrl->word_list->Count());
	ctrl->table->selection_color(FL_YELLOW);
	ctrl->table->col_header(1);
	ctrl->table->when(FL_WHEN_RELEASE);
	ctrl->table->SetCtrl(ctrl);
	win->resizable(ctrl->table);
	y += 21*input_h;
	for(int i = 0; i < 4; ++i) {
		ctrl->inpt[i] = new Fl_Input(x, y, input_w, input_h);		
		x += input_w;
	}
	ctrl->inpt[0]->maximum_size(ParsedStr::orglen);
	ctrl->inpt[1]->maximum_size(ParsedStr::trllen);
	ctrl->inpt[2]->maximum_size(ParsedStr::stlen);
	ctrl->inpt[3]->maximum_size(ParsedStr::dtlen);
	ctrl->inpt[0]->take_focus();
	ctrl->inpt[3]->color(48);
	x = frame;
	y += input_h + frame;
	ctrl->inpt[4] = new Fl_Input(frame, y, input_w, input_h);
	ctrl->inpt[4]->maximum_size(ParsedStr::orglen);
	x = frame + input_w / 2;
	y += input_h + frame;
	for(int i = 0; i < 6; ++i) {
		ctrl->b[i] = new Fl_Button(x, y, button_w, button_h, bname[i]);
		ctrl->b[i]->box(FL_PLASTIC_UP_BOX);
		x += frame + button_w;
	}
	y += frame + 2*input_h;
	ctrl->msg = new Fl_Output(frame, y, 4*input_w, input_h);
	ctrl->msg->box(FL_FLAT_BOX);
	ctrl->msg->color(FL_GRAY);
	ctrl->b[0]->callback(add_cb, (void*)ctrl);
	ctrl->b[0]->tooltip(_("Add new word\n'Enter'"));
	ctrl->b[0]->shortcut(FL_Enter);
	ctrl->b[1]->callback(delete_cb, (void*)ctrl);
	ctrl->b[1]->tooltip(_("Delete word\n'CTRL+d'"));
	ctrl->b[1]->shortcut(FL_CTRL+'d');
	ctrl->b[2]->callback(amend_cb, (void*)ctrl);
	ctrl->b[2]->tooltip(_("Amendment\n'CTRL+e'"));
	ctrl->b[2]->shortcut(FL_CTRL+'e');
	ctrl->b[3]->callback(find_cb, (void*)ctrl);
	ctrl->b[3]->tooltip(_("Search\n'CTRL+s'"));
	ctrl->b[3]->shortcut(FL_CTRL+'s');
	ctrl->b[4]->callback(clear_cb, (void*)ctrl);
	ctrl->b[4]->tooltip(_("Clear the input fields\n'CTRL+c'"));
	ctrl->b[4]->shortcut(FL_CTRL+'c');
	ctrl->b[5]->callback(exit_cb, (void*)ctrl);
	ctrl->b[5]->tooltip(_("Exit\n'ESC'"));
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
	rows(ctrl->word_list->Count());
	redraw();
	ctrl->inpt[0]->take_focus();
}

void add_cb(Fl_Widget *w, void *user)
{
	controls *c = (controls*)user;
	try {
		if(strcmp(c->inpt[0]->value(), "") != 0) { //for not empty input
			c->word_list->Add(c->inpt[0]->value(), c->inpt[1]->value(),
												c->inpt[2]->value());
			c->table->Refresh();
			c->msg->value("");
		}
		else
			c->msg->value(_("Enter the word"));
	}
	catch(const InputError &ie) {
		c->msg->value(ie.Comment());
	}
	c->word_list->WriteToFile();
	c->inpt[0]->take_focus();
}

void find_cb(Fl_Widget *w, void *user)
{
	controls *c = (controls*)user;
	word *begin = c->word_list->First();
	word *tmp = 0;
	try {
		if(strcmp(c->inpt[0]->value(), "") != 0) {
			while(begin != 0) {
				begin = c->word_list->Find(begin, c->inpt[0]->value(), rp_origl);
				if(begin != 0) {
					tmp = begin->next;
					c->word_list->MoveForward(begin);
					begin = tmp;
				}
			}	
		}
		else if(strcmp(c->inpt[1]->value(), "") != 0) {
			while(begin != 0) {
				begin = c->word_list->Find(begin, c->inpt[1]->value(), rp_tranl);
				if(begin != 0) {
					tmp = begin->next;
					c->word_list->MoveForward(begin);
					begin = tmp;
				}
			}	
		}
		else if(strcmp(c->inpt[2]->value(), "") != 0) {
			while(begin != 0) {
				begin = c->word_list->Find(begin, c->inpt[2]->value(), rp_st);
				if(begin != 0) {
					tmp = begin->next;
					c->word_list->MoveForward(begin);
					begin = tmp;
				}
			}	
		}
		else if(strcmp(c->inpt[3]->value(), "") != 0) {
			while(begin != 0) {
				begin = c->word_list->Find(begin, c->inpt[3]->value(), rp_dt);
				if(begin != 0) {
					tmp = begin->next;
					c->word_list->MoveForward(begin);
					begin = tmp;
				}
			}	
		}
	}
	catch(const InputError &ie) {
		c->msg->value(ie.Comment());
	}
	c->table->redraw();
	c->table->row_position(0);
	c->inpt[0]->take_focus();
}

void amend_cb(Fl_Widget *w, void *user)
{
	controls *c = (controls*)user;
	try {
		if(strcmp(c->inpt[0]->value(), "") != 0) {
			if(strcmp(c->inpt[4]->value(), "") != 0) {
				c->word_list->Amend(c->inpt[0]->value(), c->inpt[4]->value(), rp_origl);
				c->inpt[0]->value(c->inpt[4]->value());
				c->inpt[4]->value("");
			}
			if(strcmp(c->inpt[1]->value(), "") != 0)
				c->word_list->Amend(c->inpt[0]->value(), c->inpt[1]->value(), rp_tranl);
			if(strcmp(c->inpt[2]->value(), "") != 0)
				c->word_list->Amend(c->inpt[0]->value(), c->inpt[2]->value(), rp_st);
			c->table->Refresh();
			c->msg->value("");
			c->inpt[0]->take_focus();
		}
		else
			throw InputError(6, _("Original"), _("Empty input"));
	}
	catch(const InputError &ie) {
		c->msg->value(ie.Comment());
	}
	c->word_list->WriteToFile();
	c->inpt[0]->take_focus();
}

void delete_cb(Fl_Widget *w, void *user)
{
	controls *c = (controls*)user;
	try {
		if(strcmp(c->inpt[0]->value(), "") != 0) {
			c->word_list->Delete(c->inpt[0]->value());
			c->table->Refresh();
			c->msg->value("");
		}
		else
			throw InputError(6, _("Original"), _("Empty input"));
	}
	catch(const InputError &ie) {
		c->msg->value(ie.Comment());
	}
	c->word_list->WriteToFile();
	c->inpt[0]->take_focus();
}

void clear_cb(Fl_Widget *w, void *user)
{
	controls *c = (controls*)user;
	for(int i = 0; i < 5; ++i) {
		c->inpt[i]->value("");
	}
	c->msg->value("");
	c->inpt[0]->take_focus();
}

void DTable::sort_col(enum reqpart col, bool rev)
{
	select_all_rows(0); //disable row selection
	ctrl->word_list->Sort(col, rev);
	Refresh();
}
