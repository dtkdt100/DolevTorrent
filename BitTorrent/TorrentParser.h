#pragma once
#include "Defs.h"
#include "BenCodeObject.h"
#include "bencode.h"
#include "Sha1.h"

const size_t BLOCKSIZE = 16384; // 2^14

class TorrentParser {
public:
	static std::string infoHash(BenCodeObject& torrent);
	static size_t numberPieces(BenCodeObject& torrent);
	static size_t fileSize(BenCodeObject& torrent);
	static size_t pieceLength(BenCodeObject& torrent, int pieceIndex);
	static size_t blocksPerPiece(BenCodeObject& torrent, int pieceIndex);
	static size_t blockLen(BenCodeObject& torrent, int pieceIndex, int blockIndex);
	static BenCodeObject getInfo(BenCodeObject& torrent);
	static std::string infoHash(std::string& torrent);
};

