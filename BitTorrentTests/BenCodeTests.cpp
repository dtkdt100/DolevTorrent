#include "pch.h"
#include "BitTorrent/bencode.h"

TEST(BenCodeTest, TestIntegerType) {
	std::string bencodeStr = "i9e";
	BenCodeObject res = bencode::decode(bencodeStr);
	EXPECT_EQ(res.bType, BenCode_Integer);
}

TEST(BenCodeTest, TestIntegerValue) {
	std::string bencodeStr = "i-9e";
	BenCodeObject res = bencode::decode(bencodeStr);
	EXPECT_EQ(res.getInteger(), -9);
}

TEST(BenCodeTest, TestEncodeInteger) {
	BenCodeObject obj = BenCodeObject(-9);
	std::string res = bencode::encode(obj);
	EXPECT_EQ(res, "i-9e");
}