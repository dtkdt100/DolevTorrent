#pragma once
#include "Exception.h"
#include "HexUtils.h"
#include "BenCodeObject.h"
#include "Pieces.h"

enum MessagesTypes {
	choke = 0,
	unchoke = 1,
	interested = 2,
	not_interested = 3,
	have = 4,
	bitfield = 5,
	request = 6,
	piece = 7,
	cancel = 8,
	port = 9,
	extended = 20
};

class Messages {
public:
	static std::string buildHandShake(Buffer& infoHash, Buffer& peerId);
	static std::string buildKeepAlive();
	static std::string buildChoke();
	static std::string buildUnchoke();
	static std::string buildInterested();
	static std::string buildUnInterested();
	static std::string buildHave(int pieceIndex);
	static std::string buildBitfield(std::vector<bool> bitfield);
	static std::string buildRequest(PieceInfo info);
	static std::string buildPiece(int pieceIndex, int blockIndex, std::string& block);
	static std::string buildCancel(int pieceIndex, int blockIndex);
	static std::string buildPort(int port);

	static BenCodeDictionary<BenCodeObject> parseResponse(std::string &msg);
};

