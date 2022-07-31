#pragma once
#include "Defs.h"
#include "BenCodeObject.h"
#include "bencode.h"
#include "Sha1.h"

class TorrentParser {
public:
	static std::string infoHash(BenCodeObject& torrent);
	static size_t fileSize(BenCodeObject& torrent);
private:
	static BenCodeObject getInfo(BenCodeObject& torrent);
	static std::string infoHash(std::string&& torrent);
	
};

