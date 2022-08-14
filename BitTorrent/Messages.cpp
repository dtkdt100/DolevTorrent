#include "Messages.h"

std::string Messages::buildHandShake(Buffer& infoHash, Buffer& peerId) {
	Buffer buf;
	// pstrlen
	HexUtils::writeUInt8(&buf, 19);
	// pstr
	HexUtils::writeString(&buf, "BitTorrent protocol");
	// reserved
	HexUtils::writeUInt64BE(&buf, 0);
	// info hash
	HexUtils::writeBytes(&buf, infoHash);
	// peer id
	HexUtils::writeBytes(&buf, peerId);

	return std::string(buf.begin(), buf.end());
}

std::string Messages::buildKeepAlive() {
	Buffer buf;
	HexUtils::writeUInt32BE(&buf, 0);
	return std::string(buf.begin(), buf.end());
}

std::string Messages::buildChoke() {
	Buffer buf;
	// length
	HexUtils::writeUInt32BE(&buf, 1);
	// id
	HexUtils::writeUInt8(&buf, 0);
	return std::string(buf.begin(), buf.end());
}

std::string Messages::buildUnchoke() {
	Buffer buf;
	// length
	HexUtils::writeUInt32BE(&buf, 1);
	// id
	HexUtils::writeUInt8(&buf, 1);
	return std::string(buf.begin(), buf.end());
}

std::string Messages::buildInterested() {
	Buffer buf;
	// length
	HexUtils::writeUInt32BE(&buf, 1);
	// id
	HexUtils::writeUInt8(&buf, 2);
	return std::string(buf.begin(), buf.end());
}

std::string Messages::buildUnInterested() {
	Buffer buf;
	// length
	HexUtils::writeUInt32BE(&buf, 1);
	// id
	HexUtils::writeUInt8(&buf, 3);
	return std::string(buf.begin(), buf.end());
}

std::string Messages::buildHave(int pieceIndex)
{
	return std::string();
}

std::string Messages::buildBitfield(std::vector<bool> bitfield)
{
	return std::string();
}

std::string Messages::buildRequest(PieceInfo info) {
	Buffer buf;
	// length
	HexUtils::writeUInt32BE(&buf, 13);
	// id
	HexUtils::writeUInt8(&buf, MessagesTypes::request);
	// index
	HexUtils::writeUInt32BE(&buf, info.index);
	// begin
	HexUtils::writeUInt32BE(&buf, info.begin);
	// len
	HexUtils::writeUInt32BE(&buf, info.len);
	return std::string(buf.begin(), buf.end());
}

std::string Messages::buildPiece(int pieceIndex, int blockIndex, std::string& block)
{
	return std::string();
}

std::string Messages::buildCancel(int pieceIndex, int blockIndex)
{
	return std::string();
}

std::string Messages::buildPort(int port)
{
	return std::string();
}

BenCodeDictionary<BenCodeObject> Messages::parseResponse(std::string& msg) {
	if (msg.size() < 4) {
		throw Exception("Invalid response");
	}
	int id = msg[4];
	BenCodeDictionary<BenCodeObject> retVal;
	
	if (id < 6) {
		retVal["payload"] = BenCodeObject(msg.substr(5, msg.size() - 5));
	}
	else if (id == 6 || id == 7 || id == 8) {
		BenCodeDictionary<BenCodeObject> payload;

		payload["index"] = BenCodeObject(HexUtils::hexToInt(msg.substr(5, 4)));
		payload["begin"] = BenCodeObject(HexUtils::hexToInt(msg.substr(9, 4)));
		payload[id == 7 ? "block" : "length"] = BenCodeObject(msg.substr(13, msg.size() - 13));
		
		retVal["payload"] = BenCodeObject(BenCodeDictionary<BenCodeObject>(payload));
	}

	retVal["id"] = BenCodeObject(id);
	retVal["size"] = HexUtils::hexToInt(msg.substr(0, 4));
	return retVal;
}
