#include "BenCodeObject.h"

BenCodeObject::BenCodeObject()
{
}

BenCodeObject::BenCodeObject(BenCodeInteger v) {
	value.benCodeInteger = v;
	bType = BenCodeType::BenCode_Integer;
}

BenCodeObject::BenCodeObject(BenCodeString&& v) {
	value.benCodeString = new BenCodeString(std::move(v));
	bType = BenCodeType::BenCode_String;
}

BenCodeObject::BenCodeObject(BenCodeList<BenCodeObject>&& v) {
	value.benCodeList = new BenCodeList<BenCodeObject>(std::move(v));
	bType = BenCodeType::BenCode_List;
}

BenCodeObject::BenCodeObject(BenCodeDictionary<BenCodeObject>&& v) {
	value.benCodeDictionary = new BenCodeDictionary<BenCodeObject>(std::move(v));
	bType = BenCodeType::BenCode_Dictionary;
}

BenCodeInteger BenCodeObject::getInteger() {
	if (bType == BenCodeType::BenCode_Integer) {
		return value.benCodeInteger;
	}
	else {
		throw Exception("BenCodeObject is not an integer");
	}
}

BenCodeString BenCodeObject::getString() {
	if (bType == BenCodeType::BenCode_String) {
		return *value.benCodeString;
	}
	else {
		throw Exception("BenCodeObject is not a string");
	}
}

BenCodeList<BenCodeObject>* BenCodeObject::getList() {
	if (bType == BenCodeType::BenCode_List) {
		return value.benCodeList;
	}
	else {
		throw Exception("BenCodeObject is not a list");
	}
}

BenCodeDictionary<BenCodeObject>* BenCodeObject::getDictionary() {
	if (bType == BenCodeType::BenCode_Dictionary) {
		return value.benCodeDictionary;
	}
	else {
		throw Exception("BenCodeObject is not a dictionary");
	}
}