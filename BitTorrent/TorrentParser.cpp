#include "TorrentParser.h"
#include "HexUtils.h"

std::string TorrentParser::infoHash(BenCodeObject& torrent) {
	BenCodeObject info = getInfo(torrent);
	return infoHash(bencode::encode(info));
	
}

size_t TorrentParser::fileSize(BenCodeObject& torrent) {
	BenCodeObject info = getInfo(torrent);
	
	BenCodeDictionary<BenCodeObject> infoDict = *info.getDictionary();

	if (infoDict.find("length") != infoDict.end()) {
		return infoDict["length"].getInteger();
	}
	else if (infoDict.find("files") != infoDict.end()) {
		BenCodeList<BenCodeObject> files = *infoDict["files"].getList();
		size_t fSize = 0;
		for (auto it = files.begin(); it != files.end(); it++) {
			BenCodeDictionary<BenCodeObject> file = *it->getDictionary();
			if (file.find("length") == file.end()) {
				throw Exception("torrent file contains a file with no length");
			}
			fSize += file["length"].getInteger();
		}
		return fSize;
	}
	
	throw Exception("torrent file has no length");
}

BenCodeObject TorrentParser::getInfo(BenCodeObject& torrent) {
	if (torrent.bType != BenCodeType::BenCode_Dictionary) {
		throw Exception("torrent file is not a dictionary");
	}
	BenCodeDictionary<BenCodeObject> dict = *torrent.getDictionary();
	if (dict.find("info") == dict.end()) {
		throw Exception("torrent file has no info dictionary");
	}
	BenCodeObject& info = dict["info"];
	if (info.bType != BenCodeType::BenCode_Dictionary) {
		throw Exception("info is not a dictionary");
	}
	return info;
}

std::string TorrentParser::infoHash(std::string&& torrent) {
	Sha1 hashFun;
	hashFun.update(torrent);
	return hashFun.final();
}
