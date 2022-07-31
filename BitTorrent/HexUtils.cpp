#include "HexUtils.h"

void HexUtils::writeUInt16BE(Buffer* b, uint16_t val) {
	b->push_back((val >> 8) & 0xFF);
	b->push_back(val & 0xFF);
}

void HexUtils::writeUInt32BE(Buffer* b, uint32_t val) {
	b->push_back((val >> 24) & 0xFF);
	b->push_back((val >> 16) & 0xFF);
	b->push_back((val >> 8) & 0xFF);
	b->push_back(val & 0xFF);
}

void HexUtils::writeUInt64BE(Buffer* b, uint64_t val) {
	b->push_back((val >> 56) & 0xFF);
	b->push_back((val >> 48) & 0xFF);
	b->push_back((val >> 40) & 0xFF);
	b->push_back((val >> 32) & 0xFF);
	b->push_back((val >> 24) & 0xFF);
	b->push_back((val >> 16) & 0xFF);
	b->push_back((val >> 8) & 0xFF);
	b->push_back(val & 0xFF);
}



void HexUtils::writeBytes(Buffer* b, Buffer bVal) {
	for (uint32_t i = 0; i < bVal.size(); i++) {
		b->push_back(bVal[i]);
	}
}



uint32_t HexUtils::bufferToUInt32(Buffer& b) {
	uint32_t val = 0;
	for (int i = 0; i < 4; i++) {
		val <<= 8;
		val |= b[i];
	}
	return val;
	//return hexToInt(std::string(b.begin(), b.end()));
}

uint64_t HexUtils::bufferToUInt64(Buffer& b) {
	uint64_t val = 0;
	for (int i = 0; i < 8; i++) {
		val <<= 8;
		val |= b[i];
	}
	return val;
}

std::string HexUtils::toEexNNFormat(unsigned char& c) {
	std::string hexNNForamt = "";
	char hex[3] = { 0 };
	sprintf_s(hex, "%02x", c);
	hexNNForamt.push_back('%');
	hexNNForamt.push_back(hex[0]);
	hexNNForamt.push_back(hex[1]);
	return hexNNForamt;
}

Buffer HexUtils::hexStringToBufferHex(std::string& hexString) {
	Buffer buffer;
	for (int i = 0; i < hexString.length(); i += 2) {
		std::string hex = hexString.substr(i, 2);
		buffer.push_back(strtol(hex.c_str(), NULL, 16));
	}
	return buffer;
}

uint32_t HexUtils::hexToInt(std::string&& hexStr) {
	Buffer buf;
	for (int i = 0; i < hexStr.size(); i++) {
		buf.push_back(hexStr[i]);
	}
	return bufferToUInt32(buf);
}

uint32_t HexUtils::hexToInt(char& hexChar) {
	return (int)(hexChar);
}

uint64_t HexUtils::hexToInt64(std::string&& hexStr) {
	Buffer buf;
	for (int i = 0; i < 8; i++) {
		buf.push_back(hexStr[i]);
	}
	return bufferToUInt64(buf);
}
