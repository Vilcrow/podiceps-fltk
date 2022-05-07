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
#include <FL/Fl_Button.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Output.H>
#include "DFile.H"
#include "DHandler.H"
#include "DGraph.H"
#include "DError.H"

#define _(STR) (STR)
#define N_(STR) (STR)

int main(int argc, char** argv)
{
	ParsedStr::SetPaths();
	make_bak_file();
	try {
		if(argc == 1) {
			start_GUI();
		}
		else {
			const cl_arg *clarg = get_arguments(argc, argv);
			handle_arguments(clarg);
		}
	}
	catch(const InputError& ie) {
		fprintf(stderr, "Input error: %s (%s): %s\n", ie.String(), ie.Comment(),
				strerror(ie.Errno()));
		return 1;
	}
	catch(const FileError& fe) {
		fprintf(stderr, "File exception: %s (%s): %s\n", fe.Name(), fe.Comment(),
				strerror(fe.Errno()));
		return 1;
	}
	return 0;
}
