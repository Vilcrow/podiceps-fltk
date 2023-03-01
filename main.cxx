/*
podiceps-fltk - pocket dictionary

Copyright (C) 2022-2023 S.V.I 'Vilcrow', <vilcrow.net>
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

#include <unistd.h>
#include <string.h>
#include <getopt.h>
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

#if RUN_UTESTS
#include <CppUTest/CommandLineTestRunner.h>
#include "DUTest.cxx"
#endif

int main(int argc, char** argv)
{
#if RUN_UTESTS
	CommandLineTestRunner::RunAllTests(argc, argv);
#endif
	DFile::SetPaths(); //set paths for dictionary and backup files
	DFile::MakeBackup(); //create backup file
	struct option const longopts[] =
	{
		{ "help", no_argument, NULL, 'h' },
		{ "add", required_argument, NULL, 'a' },
		{ "delete", required_argument, NULL, 'd'},
		{ "amend-original", required_argument, NULL, 'O'},
		{ "amend-translation", required_argument, NULL, 'T'},
		{ "change-status", required_argument, NULL, 'S'},
		{ "show-original", required_argument, NULL, 'o'},
		{ "show-translation", required_argument, NULL, 't'},
		{ "show-status", required_argument, NULL, 's'},
		{ "show-date", required_argument, NULL, 'D'},
		{ "count", no_argument, NULL, 'c'},
		{ "sort-original", no_argument, NULL, 'r'},
		{ "sort-translation", no_argument, NULL, 'R'},
		{ "sort-status", no_argument, NULL, 'x'},
		{ "sort-date", no_argument, NULL, 'X'},
		{ "invert", no_argument, NULL, 'i'},
		{NULL, 0, NULL, 0}
	};
	try {
		if(argc == 1) { //run GUI
			start_GUI();
		}
		else { //CLI
			int opt;
			opterr = 0; //getopt doesn't print any messages
			WordList word_list;
			while((opt = getopt_long(argc, argv,
							N_("ha:d:O:T:S:o:t:s:D:crRxXi"), longopts, NULL)) != -1) {
				switch(opt) {
				case 'h': //show help
					help_page();
					return 0;
				case 'a': {	//add new word
					ParsedStr *ps = new ParsedStr(optarg, argv[optind]);
					if(argv[optind+1] != 0)
						ps->WStatus(argv[optind+1]);
					word_list.Add(ps);
					delete ps;
					word_list.WriteToFile();
					return 0;
				}
				case 'd': //delete word
					word_list.Delete(optarg);	
					return 0;
				case 'O': //change original word
					word_list.Amend(optarg, argv[optind], rp_origl);
					return 0;
				case 'T': //change translation
					word_list.Amend(optarg, argv[optind], rp_tranl);
					return 0;
				case 'S': //change status
					word_list.Amend(optarg, argv[optind], rp_st);
					return 0;
				case 'o': { //print all words matching the original pattern
					word *tmp = word_list.First();
					while(tmp != 0) {
						tmp = word_list.Find(tmp, optarg, rp_origl);
						if(tmp != 0) {
							print_word(tmp->ps.SourceString());
							tmp = tmp->next;
						}
					}
					return 0;
				}
				case 't': {  //print all words matching the translation pattern
					word *tmp = word_list.First();
					while(tmp != 0) {
						tmp = word_list.Find(tmp, optarg, rp_tranl);
						if(tmp != 0) {
							print_word(tmp->ps.SourceString());
							tmp = tmp->next;
						}
					}
					return 0;
				}
				case 's': {  //print all words matching the status pattern
					word *tmp = word_list.First();
					while(tmp != 0) {
						tmp = word_list.Find(tmp, optarg, rp_st);
						if(tmp != 0) {
							print_word(tmp->ps.SourceString());
							tmp = tmp->next;
						}
					}
					return 0;
				}
				case 'D': {  //print all words matching the date pattern
					word *tmp = word_list.First();
					while(tmp != 0) {
						tmp = word_list.Find(tmp, optarg, rp_dt);
						if(tmp != 0) {
							print_word(tmp->ps.SourceString());
							tmp = tmp->next;
						}
					}
					return 0;
				}
				case 'c': //print the total count of words in dictionary
					printf(_("Total count: %d\n"), word_list.Count());
					return 0;
				case 'r': //resort by original
					word_list.Sort(rp_origl);
					return 0;
				case 'R': //resort by translation
					word_list.Sort(rp_tranl);
					return 0;
				case 'x': //resort by status
					word_list.Sort(rp_st);
					return 0;
				case 'X': //resort by date
					word_list.Sort(rp_dt);
					return 0;
				case 'i': //invert list
					word_list.Reverse();
					word_list.WriteToFile();
					return 0;
				default: //incorrect argument
					help_page();
					exit(1);
				}
			}
		}
	}
	//for critical errors
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
