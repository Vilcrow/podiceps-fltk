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

#include <string.h>
#include <errno.h>
#include "DError.H"

char* strcopy(const char *str)
{
	char *res = new char[strlen(str)+1];
	strcpy(res, str);
	return res;
}

ExternalError::ExternalError()
{
	err_code = errno;
}

FileError::FileError(const char *fn, const char *cm) : ExternalError()
{
	filename = strcopy(fn);
	comment = strcopy(cm);
}

FileError::~FileError()
{
	delete [] filename;
	delete [] comment;
}

FileError::FileError(const FileError& other)
{
	err_code = other.err_code;
	filename = strcopy(other.filename);
	comment = strcopy(other.comment);
}

InputError::InputError(int ec, const char *str, const char *cm)
{
	err_code = ec;
	string = strcopy(str);
	comment = strcopy(cm);
}

InputError::~InputError()
{
	delete [] string;
	delete [] comment;
}

InputError::InputError(const InputError& other)
{
	err_code = other.err_code;
	string = strcopy(other.string);
	comment = strcopy(other.comment);
}
