#pragma once
#include "BenCodeObject.h"



class bencode {
	// Bencode (pronounced like Bee-encode) is the encoding used by the peer-to-peer 
	// file sharing system BitTorrent for storing and transmitting loosely structured data.
	// This class can decode a string that is decoded with bencode or encode it.
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

