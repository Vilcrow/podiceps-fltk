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

#ifndef DERROR_H_SENTRY
#define DERROR_H_SENTRY

char *strcopy(const char *str);

class DError {

};

class UserError : public DError {

};

class InputError : public UserError {
	int err_code;
	char *string;
	char *comment;
public:
	InputError(int ec, const char *str, const char *cm);
	~InputError();
	InputError(const InputError& other);
	inline const char *String() const { return string; }
	inline const char *Comment() const { return comment; }
	inline int Errno() const { return err_code; }
};

class ExternalError : public DError {
public:
	ExternalError();
	~ExternalError() {}
protected:
	int err_code;
};

class FileError : public ExternalError {
	char *filename;
	char *comment;
public:
	FileError(const char* fn, const char* cm);
	~FileError();
	FileError(const FileError& other);
	inline const char *Name() const { return filename; }
	inline const char *Comment() const { return comment; }
	inline int Errno() const { return err_code; }
};

#endif
