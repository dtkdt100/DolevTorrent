#pragma once
#include "Exception.h"

class FilePathUtils {
public:
	FilePathUtils() = delete;
	static std::string getFileBasePath(const std::string& filePath);
	static std::string appendPath(const std::string& path1, const std::string& path2);
	static std::string getFileName(const std::string& filePath);
	static std::string generateFilePath(const std::string& path, const std::string& filename, const std::string& ending);
private:
	static bool isEndOfFilePath(const std::string::size_type& pos);
};

