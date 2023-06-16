#include <iostream>
#include "bencode.h"
#include "HexUtils.h"
#include "Exception.h"

BenCodeObject bencode::decode(std::string& decodedStr) {
	int index = 0;
	return decode(decodedStr, &index);
}

std::string bencode::encode(BenCodeObject obj) {
	if (obj.bType == BenCodeType::BenCode_Integer) {
		return encodeInteger(obj.getInteger());
	}
	else if (obj.bType == BenCodeType::BenCode_List) {
		return encodeList(*obj.getList());
	}
	else if (obj.bType == BenCodeType::BenCode_Dictionary) {
		return encodeDictionary(*obj.getDictionary());
	}
	else {
		return encodeString(obj.getString());
	}
}

BenCodeObject bencode::decode(std::string& decodedStr, int* i) {
	if (decodedStr[*i] == 'i') {
		return BenCodeObject(decodeInteger(decodedStr, i));
	} else if (decodedStr[*i] == 'l') {
		return BenCodeObject(decodeList(decodedStr, i));
	} else if (decodedStr[*i] == 'd') {
		return BenCodeObject(decodeDictionary(decodedStr, i));
	} else {
		return BenCodeObject(decodeString(decodedStr, i));
	}
}

BenCodeInteger bencode::decodeInteger(std::string& decodedStr, int* i) {
	*i += 1;
	BenCodeInteger retValue = getInBenCodeInteger(decodedStr, i);
	return BenCodeInteger(retValue);
}

BenCodeString bencode::decodeString(std::string& decodedStr, int* i) {
	// <length>:<contents>: 4:spam -> spam
	BenCodeInteger numberOfBytes = getInBenCodeInteger(decodedStr, i, ':');
	
	BenCodeString retValue;
	
	for (int j = 0; j < numberOfBytes; j++) {
		retValue.push_back(decodedStr[*i]);
		*i += 1;
	}
	
	return retValue;
}

BenCodeList<BenCodeObject> bencode::decodeList(std::string& decodedStr, int* i) {
	//  l<contents>e: l4:spami42ee -> [spam, 42]
	BenCodeList<BenCodeObject> retValue;
	*i += 1;
	while (decodedStr[*i] != 'e') {
		retValue.push_back(decode(decodedStr, i));
	}
	*i += 1;
	return retValue;
}

BenCodeDictionary<BenCodeObject> bencode::decodeDictionary(std::string& decodedStr, int* i) {
	// starts with d and ends with 3: d3:bar4:spam3:fooi42ee -> {"bar": "spam", "foo": 42}
	BenCodeDictionary<BenCodeObject> retValue;
	*i += 1;
	while (decodedStr[*i] != 'e') {
		BenCodeString key = decodeString(decodedStr, i);
		retValue[key] = decode(decodedStr, i);
	}
	*i += 1;
	return retValue;
}

BenCodeInteger bencode::getInBenCodeInteger(std::string& decodedStr, int* i, char ending) {
	// The interager starts with an i and ends with e. for exanple: i42e -> 42
	int retValue = 0;
	int sign = 1;

	if (decodedStr[*i] == '-') {
		sign = -1;
		*i += 1;
	}

	while (decodedStr[*i] != ending) {
		retValue = retValue * 10 + (decodedStr[*i] - '0');
		*i += 1;
		if (*i >= decodedStr.size()) {
			throw Exception(INVALID_BENCODE_STRING);
		}
	}
	*i += 1;
	return retValue * sign;
	return BenCodeInteger(retValue);
}


std::string bencode::encodeInteger(BenCodeInteger obj) {
	std::string retValue = "i" + std::to_string(obj) + "e";
	return retValue;
}

std::string bencode::encodeString(BenCodeString obj) {
	std::string retValue = std::to_string(obj.size()) + ":";
	for (int i = 0; i < obj.size(); i++) {
		retValue += obj[i];
	}
	return retValue;
}

std::string bencode::encodeList(BenCodeList<BenCodeObject> obj) {
	std::string retValue = "l";
	for (BenCodeObject& o : obj) {
		retValue += encode(o);
	}
	retValue += "e";
	return retValue;
}

std::string bencode::encodeDictionary(BenCodeDictionary<BenCodeObject> obj) {
	std::string retValue = "d";
	for (auto& pair : obj) {
		retValue += encodeString(pair.first);
		retValue += encode(pair.second);
	}
	retValue += "e";
	return retValue;
}

