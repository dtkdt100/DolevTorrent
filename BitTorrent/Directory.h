#pragma once
#include "File.h"
#include "FilePathUtils.h"

class Directory {
public:
	Directory(std::string& path);
	Directory(std::string&& path);
	File addFile(std::string& fileName);
private:
	void createDir();
	
	std::string path;
};

