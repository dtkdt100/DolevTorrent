#include <iostream>
#include "CryptoGen.h"
#include "HexUtils.h"

uint32_t CryptoGen::randomBytes(size_t bSize) {
	Buffer ran = randomBytesBuffer(bSize);
	return HexUtils::bufferToUInt32(ran);
}

Buffer CryptoGen::randomBytesBuffer(size_t bSize) {
	unsigned x = randomGen();
	Buffer b(bSize);
	for (size_t i = 0; i < bSize; i++) {
		b[i] = x & 0xFF;
		x >>= 8;
		if (x == 0) {
			x = randomGen();
		}
	}
	return b;
}

unsigned CryptoGen::randomGen() {
	std::random_device engine;
	return engine();
}
