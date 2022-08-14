#include "FilePathUtils.h"

std::string FilePathUtils::getFileBasePath(const std::string& filename) {
	std::string::size_type pos = filename.find_last_of("\\");

	if (isEndOfFilePath(pos)) {
		throw std::exception(INVALID_FILE_NAME_EXCEPTION);
	}

	return filename.substr(0, pos);
}

std::string FilePathUtils::getFileName(const std::string& filePath) {
	std::string::size_type pos = filePath.find_last_of("\\");

	if (isEndOfFilePath(pos)) {
		return filePath;
	}

	return filePath.substr(pos + 1);
}

std::string FilePathUtils::generateFilePath(const std::string& path, const std::string& filename, const std::string& ending) {
	return appendPath(path, filename) + "." + ending;
}

std::string FilePathUtils::appendPath(const std::string& path1, const std::string& path2) {
	return path1 + "\\" + path2;
}



bool FilePathUtils::isEndOfFilePath(const std::string::size_type& pos) {
	return pos == std::string::npos;
}
