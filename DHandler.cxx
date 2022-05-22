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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "DFile.H"
#include "DHandler.H"
#include "DError.H"

void help_page()
{
	printf(_("Options:\n"));
	printf(_("-h - Show this help.\n")); 
	printf(_("-a [original] [translation] - Add new word.\n")); 
	printf(_("-d [original] - Delete word by original.\n")); 
	printf(_("-O [old original] [new original] - To correct the original.\n"));
	printf(_("-T [original] [new translation]- To correct the translation.\n")); 
	printf(_("-S [original] [new status] - Change status.\n")); 
	printf(_("-o [original],\n")); 
	printf(_("-t [translation],\n")); 
	printf(_("-s [status],\n")); 
	printf(_("-D [date] - Show all words matching the pattern. You can use \"*\" and \"?\".\n")); 
	printf(_("-c - Count the number of words.\n")); 
	printf(_("-r - Sort by original.\n")); 
	printf(_("-R - Sort by translation.\n")); 
	printf(_("-x - Sort by status.\n")); 
	printf(_("-X - Sort by date.\n")); 
	printf(_("Without arguments - To run GUI.\n")); 
}

void print_word(const char* str)
{
	ParsedStr ps(str);
	printf("%s", ps.Original());
	int len = strlen(ps.Original());
	for(int i = ParsedStr::orglen-len-1; i >= 0; i--)
		printf(" ");
	printf("%s", ps.Translation());		//curved columns with cyrillic. Need fix
	len = strlen(ps.Translation())/2;
	for(int i = ParsedStr::trllen/2-len-1; i >= 0; i--)
		printf(" ");
	printf("%s", ps.WStatus());
	len = strlen(ps.WStatus());
	for(int i = ParsedStr::stlen-len-1; i >= 0; i--)
		printf(" ");
	printf("%s\n", ps.Date());
}

bool is_match(const char* str, const char* pattern) //recursive function
{
	int i;
	for(;; str++, pattern++) {
		switch(*pattern) {
			case 0:
				return *str == 0;
			case '*':
				for(i = 0; ; i++) {
					if(is_match(str+i, pattern+1))
						return true;
					if(!str[i])
						return false;
				}
			case '?':
				if(!*str)
					return false;
				break;
			default:
				if(*str != *pattern)
					return false;
		}
	}
}
