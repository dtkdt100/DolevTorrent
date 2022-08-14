#include "Tracker.h"


Tracker::Tracker(std::string host, int port): sock(SOCK_DGRAM, IPPROTO_UDP) {
	sock.connectSocket(host, port);
}

std::vector<PeerInfo> Tracker::getPeers(BenCodeObject& torrent) {
	std::string connReq = buildConnReq();
	sock.sendMessage(connReq);
	std::string response = sock.receiveMessage(1024);
	analyzeConnRespose(response);
	std::string annoReq = buildAnnounceReq(torrent);
	sock.sendMessage(annoReq);
	response = sock.receiveMessage(1024);
	return analyzeAnnounceRespose(response);
}

void Tracker::analyzeConnRespose(std::string& response) {
	if (response.size() != 16) {
		throw Exception("Not a valid connection response");
	}
	for (int i = 0; i < 4; i++) {
		if (response[i] != '\0') {
			throw Exception("Not a valid connection response");
		}
	}
	if (transactionId != HexUtils::hexToInt(response.substr(4, 8))) {
		throw Exception("Not a valid connection response");
	}
	connectionId = HexUtils::hexToInt64(response.substr(8, 8));
}

std::vector<PeerInfo> Tracker::analyzeAnnounceRespose(std::string& response) {
	if (response.size() < 20) {
		throw Exception("Not a valid announce response");
	}
	
	if (0x0001 != HexUtils::hexToInt(response.substr(0, 8))) {
		throw Exception("Not a valid announce response");
	}
	
	if (transactionId != HexUtils::hexToInt(response.substr(4, 8))) {
		throw Exception("Not a valid announce response");
	}

	int interval = HexUtils::hexToInt(response.substr(8, 8));
	int leechers = HexUtils::hexToInt(response.substr(12, 8));
	int seeders = HexUtils::hexToInt(response.substr(16, 8));
 	std::vector<PeerInfo> peers;
	for (int i = 20; i < response.size(); i += 6) {
		PeerInfo p;
		std::string ip = "";
		
		for (int j = 0; j < 4; j++) {
			unsigned char c = response[i + j];
			std::string s  = std::to_string(c);
			
			for (int k = 0; k < s.size(); k++) {
				ip.push_back(s[k]);
			}
			
			if (j != 3) ip.push_back('.');
		}
		p.ip = ip;
		p.port = HexUtils::hexToInt16(response.substr(i + 4, 2));
		p.id = peerId;
		peers.push_back(p);
	}
	return peers;
}

std::string Tracker::buildConnReq() {	
	Buffer buf;
	// connection id
	HexUtils::writeUInt32BE(&buf, 0x417); 
	HexUtils::writeUInt32BE(&buf, 0x27101980);
	// action
	HexUtils::writeUInt32BE(&buf, 0);
	// transaction id
	transactionId = CryptoGen::randomBytes(4);
	HexUtils::writeUInt32BE(&buf, transactionId);
	
	return std::string(buf.begin(), buf.end());
}

std::string Tracker::buildAnnounceReq(BenCodeObject& torrent) {
	Buffer buf;
	// connection id
	HexUtils::writeUInt64BE(&buf, connectionId);
	// action
	HexUtils::writeUInt32BE(&buf, 1);
	// transaction id
	transactionId = CryptoGen::randomBytes(4);
	HexUtils::writeUInt32BE(&buf, transactionId);
	// info hash
	std::string info = TorrentParser::infoHash(torrent);
	infoHash = HexUtils::hexStringToBufferHex(info);
	HexUtils::writeBytes(&buf, infoHash);
	// peer id
	if (peerId.size() == 0) {
		generatePeerId();
	}
	HexUtils::writeBytes(&buf, peerId);
	// downloaded
	HexUtils::writeUInt64BE(&buf, 0);
	// left
	HexUtils::writeUInt64BE(&buf, TorrentParser::fileSize(torrent));
	// uploaded
	HexUtils::writeUInt64BE(&buf, 0);
	// event
	HexUtils::writeUInt32BE(&buf, 0);
	// ip address
	HexUtils::writeUInt32BE(&buf, 0);
	// key
	HexUtils::writeUInt32BE(&buf, CryptoGen::randomBytes(4));
	// num want
	HexUtils::writeUInt32BE(&buf, -1);
	// port
	HexUtils::writeUInt16BE(&buf, 6881);
	
	return std::string(buf.begin(), buf.end());
}

void Tracker::generatePeerId() {
	peerId.push_back('F'); // Franco
	peerId.push_back('T'); // Torrent
	peerId.push_back('2'); // 2.0
	Buffer randomBytes = CryptoGen::randomBytesBuffer(17);
	for (int i = 0; i < randomBytes.size(); i++) {
		peerId.push_back(randomBytes[i]);
	}
}


