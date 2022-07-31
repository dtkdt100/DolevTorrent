#pragma once
#include <random>
#include "Defs.h"



class CryptoGen {
public:
	static uint32_t randomBytes(size_t bSize);
	static Buffer randomBytesBuffer(size_t bSize);
private:
	static unsigned randomGen();
};

