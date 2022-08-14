#pragma once
#include "Defs.h"

class HexUtils {
public:
	static void writeUInt8(Buffer* b, uint8_t val);
	static void writeUInt16BE(Buffer* b, uint16_t val);
	static void writeUInt32BE(Buffer *b, uint32_t val);
	static void writeUInt64BE(Buffer *b, uint64_t val);
	static void writeBytes(Buffer* b, Buffer bVal);
	static void writeString(Buffer* b, std::string&& s);

	static uint16_t bufferToUInt16(Buffer& b);
	static uint32_t bufferToUInt32(Buffer& b);
	static uint64_t bufferToUInt64(Buffer& b);
	static std::string toEexNNFormat(unsigned char& c);
	static Buffer hexStringToBufferHex(std::string& hexString);
	static uint32_t hexToInt(std::string&& hexStr);
	static uint32_t hexToInt16(std::string&& hexStr);
	static uint32_t hexToInt(char& hexChar);
	static uint64_t hexToInt64(std::string&& hexStr);
};

