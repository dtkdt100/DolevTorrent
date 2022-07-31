#pragma once
#include "Defs.h"

using ExceptionType = char const* const;

ExceptionType NO_EXCEPTION = "";

ExceptionType FILE_EXSITS_EXCEPTION = "File already exists";
ExceptionType FILE_NOT_EXSITS_EXCEPTION = "File does not exist";

ExceptionType WRITE_TO_FILE_EXCEPTION = "Error writing to file";
ExceptionType RENAME_FILE_Exception = "Error renaming file";
ExceptionType DELETE_FILE_EXCEPTION = "Error deleting file";
ExceptionType SET_START_FILE_POINTER_EXCEPTION = "Error setting start file pointer";
ExceptionType READ_FROM_FILE_EXCEPTION = "Error reading from file";

ExceptionType INAVILD_HANDLE_EXCEPTION = "Invalid handle";
ExceptionType INVALID_FILE_NAME_EXCEPTION = "Invalid file name";

ExceptionType INVALID_BENCODE_STRING = "Invalid bencode string";



class Exception {
public:
	Exception(ExceptionType eType);
	void printException();
private:
	ExceptionType eType;
};

