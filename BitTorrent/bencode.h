#pragma once
#include "BenCodeObject.h"



class bencode {
public:
	static BenCodeObject decode(std::string& decodedStr);
	static std::string encode(BenCodeObject obj);

private:
	static BenCodeObject decode(std::string& decodedStr, int* i);
	static BenCodeInteger decodeInteger(std::string& decodedStr, int* i);
	static BenCodeString decodeString(std::string& decodedStr, int* i);
	static BenCodeList<BenCodeObject> decodeList(std::string& decodedStr, int* i);
	static BenCodeDictionary<BenCodeObject> decodeDictionary(std::string& decodedStr, int* i);
	static BenCodeInteger getInBenCodeInteger(std::string& decodedStr, int* i, char ending = 'e');

	static std::string encodeInteger(BenCodeInteger obj);
	static std::string encodeString(BenCodeString obj);
	static std::string encodeList(BenCodeList<BenCodeObject> obj);
	static std::string encodeDictionary(BenCodeDictionary<BenCodeObject> obj);
	
};

