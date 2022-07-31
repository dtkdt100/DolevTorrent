#include <sstream>
#include "File.h"

File::File(const std::string& path, bool createNew) :
	m_file(openFileInternal(path, createNew)) {

}

Buffer File::read(uint32_t numberOfBytes, LONG startOffset) {
	Buffer data(numberOfBytes);
	DWORD outBytes = 0;

	if (SetFilePointer(m_file.getRawHandle(), startOffset, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER) {
		throw Exception(SET_START_FILE_POINTER_EXCEPTION);
	}

	if (!ReadFile(m_file.getRawHandle(), data.data(), numberOfBytes, &outBytes, nullptr)) {
		throw Exception(READ_FROM_FILE_EXCEPTION);
	}

	if (outBytes != numberOfBytes) {
		throw Exception(READ_FROM_FILE_EXCEPTION);
	}

	return data;
}

Buffer File::readAll() {
	return read(getSize(), 0);
}

LinesBuffer File::readLines(uint32_t numberOfBytes, LONG startOffset) {
	LinesBuffer lines;
	Buffer data1 = read(numberOfBytes, startOffset);
	std::string line;
	std::stringstream stringstream;
	std::copy(data1.begin(), data1.end(), std::ostream_iterator<unsigned char>(stringstream));

	while (std::getline(stringstream, line)) {
		line.erase(line.end() - 1);
		lines.push_back(line);
	}

	return lines;
}

void File::write(const Buffer& buffer) {
	DWORD outBytes = 0;
	if (!WriteFile(m_file.getRawHandle(), buffer.data(), buffer.size(), &outBytes, nullptr)) {
		throw Exception(WRITE_TO_FILE_EXCEPTION);
	}

	if (outBytes != buffer.size()) {
		throw Exception(WRITE_TO_FILE_EXCEPTION);
	}
}

void File::writeLines(const LinesBuffer& buffer) {
	Buffer b;
	for (const auto& line : buffer) {
		for (const auto& c : line) {
			b.push_back(c);
		}
		b.push_back('\r');
		b.push_back('\n');
	}
	write(b);
}


int File::getSize() {
	DWORD fileSize = GetFileSize(m_file.getRawHandle(), nullptr);
	return fileSize;
}

bool File::compareFiles(File& other) {
	DWORD fileSize = getSize();
	DWORD otherFileSize = other.getSize();
	if (fileSize != otherFileSize) {
		return false;
	}
	Buffer data1 = read(fileSize, 0);
	Buffer data2 = other.read(otherFileSize, 0);
	return std::equal(data1.begin(), data1.end(), data2.begin());
}

void File::close() {
	m_file.close();
}

void File::rename(const std::string& oldPath, const std::string& newPath) {
	// File needs to be closed for rename to work
	if (!MoveFileA(oldPath.c_str(), newPath.c_str())) {
		throw std::exception(RENAME_FILE_Exception);
	}
}

bool File::exsits(const std::string& filePath) {
	DWORD dwAttrib = GetFileAttributesA(filePath.c_str());

	return (dwAttrib != INVALID_FILE_ATTRIBUTES &&
		!(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}

void File::deleteFile(const std::string& filePath) {
	if (!File::exsits(filePath)) return;
	if (!DeleteFileA(filePath.c_str())) {
		throw Exception(DELETE_FILE_EXCEPTION);
	}
}

bool File::compareFiles(const std::string& path1, const std::string& path2) {
	File file1(path1);
	File file2(path2);
	return file1.compareFiles(file2);
}

Handle File::openFileInternal(const std::string& filePath, bool createNew) const {
	bool fileExsits = File::exsits(filePath);
	if (createNew && fileExsits) {
		throw Exception(FILE_EXSITS_EXCEPTION);
	}
	else if (!createNew && !fileExsits) {
		throw Exception(FILE_NOT_EXSITS_EXCEPTION);
	}
	const auto fileHandle =
		CreateFileA(filePath.c_str(),
			GENERIC_READ | FILE_APPEND_DATA,
			FILE_SHARE_READ | FILE_SHARE_WRITE,
			nullptr,
			createNew ? CREATE_NEW : OPEN_EXISTING,
			0,
			nullptr);
	return Handle(fileHandle);
}


