#include "BitTorrent/Downloader.h";

int main() {	
	try {
		std::string path = "C:\\temp\\sample9.torrent";
		Downloader d(path);
	} 
	catch (Exception e) {
		e.printException();
	}

	return 0;
}