#pragma once
#include <map>
#include "Exception.h"


using BenCodeInteger = int64_t;
using BenCodeString = std::string;
template <typename T> using BenCodeList = std::vector<T>;
template <typename T> using BenCodeDictionary = std::map<std::string, T>;

enum BenCodeType {
	BenCode_Integer,
	BenCode_String,
	BenCode_List,
	BenCode_Dictionary
};

class BenCodeObject {
public:
	BenCodeType bType;

	BenCodeObject();
	BenCodeObject(BenCodeInteger v);
	BenCodeObject(BenCodeString&& v);
	BenCodeObject(BenCodeList<BenCodeObject>&& v);
	BenCodeObject(BenCodeDictionary<BenCodeObject>&& v);

	BenCodeInteger getInteger();
	BenCodeString getString();
	BenCodeList<BenCodeObject>* getList();
	BenCodeDictionary<BenCodeObject>* getDictionary();
	
private:
	union Value {
		BenCodeInteger benCodeInteger;
		BenCodeString* benCodeString;
		BenCodeList<BenCodeObject>* benCodeList;
		BenCodeDictionary<BenCodeObject>* benCodeDictionary;
	} value;
};







