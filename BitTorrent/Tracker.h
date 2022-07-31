#pragma once
#include "Socket.h"
#include "Exception.h"
#include "Defs.h"
#include "HexUtils.h"
#include "CryptoGen.h"
#include "TorrentParser.h"

struct Peer {
	std::string ip;
	int port;
};


class Tracker {
public:
	Tracker(std::string host, int port);
	std::vector<Peer> getPeers(BenCodeObject& torrent);
private:
	void analyzeConnRespose(std::string& response);
	std::vector<Peer> analyzeAnnounceRespose(std::string& response);
	std::string buildConnReq();
	std::string buildAnnounceReq(BenCodeObject& torrent);
	void generatePeerId();
	
	Socket sock;
	uint32_t transactionId;
	uint64_t connectionId;
	Buffer peerId;
};

