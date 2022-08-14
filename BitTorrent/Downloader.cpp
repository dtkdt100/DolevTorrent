#include "Downloader.h"

Downloader::Downloader(std::string& filePath): pieces(Pieces(analyzeTorrent(filePath))) {
	std::string info = TorrentParser::infoHash(torrent);
	infoHash = HexUtils::hexStringToBufferHex(info);
	BenCodeDictionary<BenCodeObject> dict = *torrent.getDictionary();
	if (dict.find("announce") != dict.end()) {
		std::string url = dict["announce"].getString();
		if (UrlUtils::getProtocol(url) == Protocol::UDP) {
			updateFromTracker(url);
		}
		else {
			backupTrackers();
		}
		if (peersNum == 0) {
			backupTrackers();
		}

	}
	else {
		throw Exception("No announce found in torrent");
	}
}

BenCodeObject& Downloader::analyzeTorrent(std::string& filePath) {
	File f(filePath);
	Buffer data123 = f.readAll();
	std::string data123str(data123.begin(), data123.end());
	torrent = bencode::decode(data123str);
	return torrent;
}

void Downloader::backupTrackers() {
	BenCodeDictionary<BenCodeObject> dict = *torrent.getDictionary();
	if (dict.find("announce-list") != dict.end()) {
		BenCodeList<BenCodeObject> list = *dict["announce-list"].getList();
		for (BenCodeList<BenCodeObject>::iterator it = list.begin(); it != list.end(); ++it) {
			for (BenCodeList<BenCodeObject>::iterator it2 = (*it).getList()->begin(); it2 != (*it).getList()->end(); ++it2) {
				std::string url = (*it2).getString();
				if (UrlUtils::getProtocol(url) == Protocol::UDP) {
					updateFromTracker(url);
				}
				if (peersNum > 0) {
					return;
				}
			}
		}
	}
}


void Downloader::updateFromTracker(std::string& url) {
	std::string hostname = UrlUtils::getDomain(url);

	std::cout << "Checking tracker: " << hostname << std::endl;
	if (hostname == "tracker.coppersurfer.tk") {
		return;
	}
	int port = UrlUtils::getPort(url);
	if (port == 0) {
		port = 80;
	}
	try {
		Tracker t(hostname, port);
		std::vector<PeerInfo> peers = t.getPeers(torrent);
		update(peers);
	}
	catch (Exception& e) {

	}
}

bool Downloader::update(std::vector<PeerInfo>& peers) {
	std::vector<HANDLE> threads;
	for (auto& peerInfo : peers) {
		try {
			auto peer = std::make_unique<Peer>(peerInfo);
			auto paramsPtr = std::make_unique<DonwloadPeerParams>(peer.release(), &pieces, infoHash);
			std::cout << "connected " << peerInfo.ip << "" << std::endl;
			HANDLE thread = CreateThread(NULL, 0,
				reinterpret_cast<LPTHREAD_START_ROUTINE>(start),
				paramsPtr.release(), 0, NULL);
			threads.push_back(thread);
			peersNum += 1;
		}
		catch (Exception e) {
			std::cout << "failed to connect " << peerInfo.ip << "" << std::endl;
		}
	}

	if (threads.size() == 0) {
		return false;
	}

	DWORD waitStatus = WaitForMultipleObjects(threads.size(), threads.data(), true, INFINITE);
	if (waitStatus < WAIT_OBJECT_0 || waitStatus >= WAIT_OBJECT_0 + threads.size()) {
		throw Exception("Error while waiting for threads to finish");
	}

	return true;
}

DWORD __stdcall Downloader::start(DonwloadPeerParams* paramsRaw) {
	try {
		std::unique_ptr<DonwloadPeerParams> mergeParams(paramsRaw);
		paramsRaw->peer->download(paramsRaw->pieces, paramsRaw->infoHash);
	}
	catch (...) {

	}
	return 0;
}
