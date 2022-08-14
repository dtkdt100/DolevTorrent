#pragma once
#include "Tracker.h"
#include "Messages.h"
#include "Pieces.h"

class Peer {
public:
	Peer(PeerInfo info);
	void download(Pieces* pieces, Buffer infoHash);
	void sendKeepAlive();
	std::string hostname;
private:
	
	void messageHandler(std::string& msg);
	bool isHandShake(std::string& msg);
	void chokeHandler();
	void unchokeHandler();
	void haveHandler(BenCodeObject payload);
	void bitfieldHandler(BenCodeObject payload);
	void pieceHandler(BenCodeObject payload);
	void requestPiece();

	Socket sock;
	Pieces* pieces;
	Buffer id;
};

