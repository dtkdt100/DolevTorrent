#include "Downloader.h"

Downloader::Downloader(std::string& filePath): pieces(Pieces(analyzeTorrent(filePath))) {
	// The functions recives a torrent file path and download the files
	std::string info = TorrentParser::infoHash(torrent);
	infoHash = HexUtils::hexStringToBufferHex(info);
	BenCodeDictionary<BenCodeObject> dict = *torrent.getDictionary();
	if (dict.find("announce") != dict.end()) {
		std::string url = dict["announce"].getString();
		if (UrlUtils::getProtocol(url) == Protocol::UDP) {
			// Find the correct Tracker, now we need to connect
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
	// From torrent file to BenCodeObject that will contain all the info in a need form.
	File f(filePath);
	Buffer file = f.readAll();
	std::string fileStr(file.begin(), file.end());
	torrent = bencode::decode(fileStr);
	return torrent;
}

void Downloader::backupTrackers() {
	// backup trackers will be called when the trackers are failed to connect or they are not
	// supported with UDP protocol
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
		// calling the tracker class. The class will handle the connect
		// If the tracker failed to connect it wil throw exception
		Tracker t(hostname, port);
		// we will get the peers form the tracker
		std::vector<PeerInfo> peers = t.getPeers(torrent);
		update(peers);
	}
	catch (Exception& e) {

	}
}

bool Downloader::update(std::vector<PeerInfo>& peers) {
	// This function will loop throw the array of peers we will try to connect
	// to each one and if we can connect, we will send the peer to a Thread 
	// and we will start downloading the wanted files.
	std::vector<HANDLE> threads;
	for (auto& peerInfo : peers) {
		try {
			auto peer = std::make_unique<Peer>(peerInfo);
			// Create a unique instance of the pieces class in order to prevent downloading the
			// same pieces from multiple peers
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

	// Waiting for all the threads to finish, witch means that file has been downloaded!
	DWORD waitStatus = WaitForMultipleObjects(threads.size(), threads.data(), true, INFINITE);
	if (waitStatus < WAIT_OBJECT_0 || waitStatus >= WAIT_OBJECT_0 + threads.size()) {
		throw Exception("Error while waiting for threads to finish");
	}

	return true;
}

DWORD __stdcall Downloader::start(DonwloadPeerParams* paramsRaw) {
	// This is the thread function, it will call a peer that will start downloading the files
	try {
		std::unique_ptr<DonwloadPeerParams> mergeParams(paramsRaw);
		paramsRaw->peer->download(paramsRaw->pieces, paramsRaw->infoHash);
	}
	catch (...) {

	}
	return 0;
}
