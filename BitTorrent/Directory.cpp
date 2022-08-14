#include "Directory.h"

Directory::Directory(std::string& path): path(path) {
	createDir();
}

Directory::Directory(std::string&& path): path(path) {
	createDir();
}

File Directory::addFile(std::string& fileName) {
	std::string completePath = FilePathUtils::appendPath(path, fileName);
	return File(completePath, true);
}

void Directory::createDir() {
	bool res = CreateDirectoryA(path.c_str(), nullptr);

	if (res == 0) {
		throw Exception("CreateDirectoryA failed");
	}
}
