#pragma once
#include "Socket.h"
#include "Exception.h"
#include "Defs.h"
#include "HexUtils.h"
#include "CryptoGen.h"
#include "TorrentParser.h"

struct PeerInfo {
	std::string ip;
	uint16_t port;
	Buffer id;
};

class Tracker {
	
public:
	Tracker(std::string host, int port);
	std::vector<PeerInfo> getPeers(BenCodeObject& torrent);

	Buffer infoHash;
	Buffer peerId;
private:
	void analyzeConnRespose(std::string& response);
	std::vector<PeerInfo> analyzeAnnounceRespose(std::string& response);
	std::string buildConnReq();
	std::string buildAnnounceReq(BenCodeObject& torrent);
	void generatePeerId();
	
	Socket sock;
	uint32_t transactionId;
	uint64_t connectionId;
};

