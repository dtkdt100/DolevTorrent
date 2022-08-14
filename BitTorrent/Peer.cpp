#include <chrono>
#include <thread>
#include "Peer.h"
#include "File.h"

Peer::Peer(PeerInfo info): sock(SOCK_STREAM, IPPROTO_TCP), hostname(info.ip), id(info.id) {
	sock.connectSocket(info.ip, info.port);
}

void Peer::download(Pieces* pieces, Buffer infoHash) {
	this->pieces = pieces;
	std::string handShakeMsg = Messages::buildHandShake(infoHash, id);
	sock.sendMessage(handShakeMsg);
	std::string response = sock.receiveMessage(68);
	
	messageHandler(response);
}

void Peer::sendKeepAlive() {
	std::string keepAliveMsg = Messages::buildKeepAlive();
	sock.sendMessage(keepAliveMsg);
}

void Peer::messageHandler(std::string& msg) {
	if (isHandShake(msg)) {
		std::string interestedMsg = Messages::buildInterested();
		sock.sendMessage(interestedMsg);
		std::string response = sock.onWholeMessage(false);
		response = sock.receiveMessage(5);
		/*if (response.size() > 5) {
			response = sock.onWholeMessage(false, 5);
		}*/
		BenCodeDictionary<BenCodeObject> res = Messages::parseResponse(response);
		int id = res["id"].getInteger();
		switch (id) {
			case MessagesTypes::choke:
				chokeHandler();
			case MessagesTypes::unchoke:
				unchokeHandler();
				break;
			case MessagesTypes::have:
				haveHandler(res["payload"]);
				break;
			case MessagesTypes::bitfield:
				bitfieldHandler(res["payload"]);
				break;
			case MessagesTypes::piece:
				pieceHandler(res["payload"]);
				break;
		}
	}
}

bool Peer::isHandShake(std::string& msg) {
	if (msg.size() < 68) {
		return false;
	}
	if (msg.substr(1, 19) != "BitTorrent protocol") {
		return false;
	}
	return true;
}

void Peer::chokeHandler() {
	sock.closeSocket();
}

void Peer::unchokeHandler() {

	requestPiece();
}

void Peer::haveHandler(BenCodeObject payload) {
}

void Peer::bitfieldHandler(BenCodeObject payload) {
}

void Peer::pieceHandler(BenCodeObject payload) {
	BenCodeDictionary<BenCodeObject> p = *payload.getDictionary();
	std::string pie = p["block"].getString();
	PieceInfo info;
	info.begin = p["begin"].getInteger();
	info.index = p["index"].getInteger();

	Buffer buf;
	for (int i = 0; i < pie.size(); i++) {
		buf.push_back(pie[i]);
	}

	pieces->addPiece(info, buf);
	pieces->printPerstage();

	if (pieces->onDone()) {
		pieces->downloadFilesToDisk();
	}
}

void Peer::requestPiece() {
	PieceInfo next = pieces->pieceNeeded();

	while (next.len != 0) {
		try {
			std::string reqPiece = Messages::buildRequest(next);

			sock.sendMessage(reqPiece);
			std::string response = sock.onWholeMessage(false);

			while (response.size() < 5) { // keep alive meesage
				response = sock.onWholeMessage(false);
			}

			BenCodeDictionary<BenCodeObject> res = Messages::parseResponse(response);
			int id = res["id"].getInteger();

			if (id == MessagesTypes::piece) {
				pieceHandler(res["payload"]);
			}			
		}
		catch (const Exception& e) {
			pieces->failedPiece(next);
			requestPiece();
		}
		next = pieces->pieceNeeded();
	}
	std::this_thread::sleep_for(std::chrono::seconds(5));
	
	if (pieces->onDone()) {
		pieces->downloadFilesToDisk();
	}

	sock.closeSocket();
}

