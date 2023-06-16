#include "BitTorrent/Downloader.h";

int main() {	
	try {
		// The torrent file path
		std::string path = "C:\\temp\\small_file.torrent";
		Downloader d(path);
	}
	catch (Exception e) {
		e.printException();
	}

	return 0;
}