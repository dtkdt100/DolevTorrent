#include <iostream>
#include "BitTorrent/Socket.h"
#include "BitTorrent/File.h"
#include "BitTorrent/bencode.h"
#include "BitTorrent/UrlUtils.h"
#include "BitTorrent/HexUtils.h"
#include <BitTorrent/CryptoGen.h>
#include <BitTorrent/Tracker.h>
#include "BitTorrent/Sha1.h"
#include "BitTorrent/TorrentParser.h"


int main() {
	File f("C:\\temp\\sample7.torrent");
	Buffer data123 = f.readAll();
	std::string data123str(data123.begin(), data123.end());
	
	try {
		BenCodeObject res = bencode::decode(data123str);
		BenCodeDictionary<BenCodeObject> dict = *res.getDictionary();

		std::string udpUrl = dict["announce"].getString();
		std::string hostname = UrlUtils::getDomain(udpUrl);
		int port = UrlUtils::getPort(udpUrl);

		Tracker t(hostname, port);
		t.getPeers(res);

	} 
	catch (Exception e) {
		e.printException();
	}

	return 0;
}