#pragma once
#include "Peer.h"
#include "UrlUtils.h"

struct DonwloadPeerParams {
	Peer* peer;
	Pieces* pieces;
	Buffer infoHash;

	DonwloadPeerParams(Peer* peer, Pieces* pieces, Buffer infoHash) {
		this->peer = peer;
		this->pieces = pieces;
		this->infoHash = infoHash;
	}
};

class Downloader {
public:
	Downloader(std::string& filePath);
private:
	BenCodeObject& analyzeTorrent(std::string& filePath);
	void backupTrackers();
	void updateFromTracker(std::string& url);
	bool update(std::vector<PeerInfo>& peers);
	
	static DWORD WINAPI start(DonwloadPeerParams* paramsRaw);
	BenCodeObject torrent;
	Pieces pieces;
	Buffer infoHash;
	
	int peersNum = 0;
};

