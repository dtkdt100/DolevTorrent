#include "Pieces.h"
#include "HexUtils.h"

Pieces::Pieces(BenCodeObject& torrent): torrent(torrent), mutexHandle(createLock()) {
	int pieceLength = TorrentParser::numberPieces(torrent);
	pieces.resize(pieceLength);
	requested.resize(pieceLength);

	for (int i = 0; i < pieceLength; i++) {
		size_t blocksPerPiece = TorrentParser::blocksPerPiece(torrent, i);
		pieces[i].resize(blocksPerPiece);
		requested[i].resize(blocksPerPiece);
		
		for (size_t j = 0; j < blocksPerPiece; j++) {
			requested[i][j] = false;
		}
	}
}

void Pieces::addPiece(PieceInfo piece, Buffer& block) {
	
	while (block.size() > BLOCKSIZE) {
		block.pop_back();
	}

	pieces[piece.index][piece.begin / BLOCKSIZE] = block;
}

void Pieces::checkPiece(int index) {
	std::string piece = "";
	for (int i = 0; i < pieces[index].size(); i++) {
		Buffer buf = pieces[index][i];
		for (int j = 0; j < buf.size(); j++) {
			piece += buf[j];
		}
	}

	BenCodeObject info = TorrentParser::getInfo(torrent);
	BenCodeDictionary<BenCodeObject> infoDict = *info.getDictionary();
	std::string piecesHash = infoDict["pieces"].getString();
	std::string sha1Piece = TorrentParser::infoHash(piece);

	for (int i = index*20; i < index*20 + 20; i++) {
		unsigned char c = piecesHash[i];
		std::string piecesHashNN = HexUtils::toEexNNFormat(c);
		for (int j = 0; j < 2; j++) {
			if (piecesHashNN[j] != sha1Piece[(i-index*20)*2+j]) {
				throw Exception("Error in piece, not the same sha1");
			}
		}
	}	
}

PieceInfo Pieces::pieceNeeded() {
	DWORD waitResult = WaitForSingleObject(
		mutexHandle.getRawHandle(),
		INFINITE);
	if (waitResult == WAIT_OBJECT_0) {
		for (int i = 0; i < pieces.size(); i++) {
			for (int j = 0; j < pieces[i].size(); j++) {
				if (pieces[i][j].size() == 0 && !requested[i][j]) {
					requested[i][j] = true;
					ReleaseMutex(mutexHandle.getRawHandle());
					return PieceInfo(i, j * BLOCKSIZE, TorrentParser::blockLen(torrent, i, j));
				}
			}
		}
	}
	
	ReleaseMutex(mutexHandle.getRawHandle());
	return PieceInfo(0, 0, 0);
}

void Pieces::failedPiece(PieceInfo info) {
	requested[info.index][info.begin/BLOCKSIZE] = false;
	pieces[info.index][info.begin / BLOCKSIZE].clear();
}

bool Pieces::onDone() {
	size_t total = TorrentParser::fileSize(torrent);
	size_t dowloadedB = downloaded();
	return dowloadedB == total;
}

void Pieces::printPerstage() {
	size_t dowloadedB = downloaded();
	size_t total = TorrentParser::fileSize(torrent);

	double_t perstage = (double)dowloadedB / (double)total * 100;
	perstage = std::round(perstage * 100) / 100;

	std::cout << "Downloded: " << perstage << "%" << std::endl;
	std::cout << "Downloaded: " << dowloadedB << " / " << total << std::endl;
}

void Pieces::downloadFilesToDisk() {
	if (startDownloadedToDisk) {
		return;
	}
	std::cout << "Downloading to disk..." << std::endl;
	startDownloadedToDisk = true;
	BenCodeObject info = TorrentParser::getInfo(torrent);
	BenCodeDictionary<BenCodeObject> infoDict = *info.getDictionary();

	Buffer allPieces;

	for (int i = 0; i < pieces.size(); i++) {
		for (int j = 0; j < pieces[i].size(); j++) {
			for (int k = 0; k < pieces[i][j].size(); k++) {
				allPieces.push_back(pieces[i][j][k]);
			}
		}
	}
	

	if (infoDict.find("files") != infoDict.end()) {
		// multifile
		std::string downloadDir = FilePathUtils::appendPath(DOWNLOADPATH, (infoDict["name"]).getString());
		Directory dir(downloadDir);

		BenCodeObject files = infoDict["files"];
		BenCodeList<BenCodeObject> filesList = *files.getList();
		int index = 0;
		for (auto file: filesList) {
			BenCodeDictionary<BenCodeObject> fileDict = *file.getDictionary();
			BenCodeObject path = fileDict["path"];
			BenCodeList<BenCodeObject> pathList = *path.getList();
			std::string pathString = "";
			for (auto p: pathList) {
				pathString += p.getString();
			}
			File f = dir.addFile(pathString);
			int fileLen = fileDict["length"].getInteger();
			Buffer fileBuf;
			int endOfFile = index + fileLen;
			for (index; index < endOfFile; index++) {
				fileBuf.push_back(allPieces[index]);
			}
			f.write(fileBuf);
			f.close();
		}
		std::cout << "Done: " << downloadDir << std::endl;
		
	} 
	else if (infoDict.find("length") != infoDict.end()) {
		// single file
		std::string downloadDir = FilePathUtils::appendPath(DOWNLOADPATH, (infoDict["name"]).getString());
		File f(downloadDir, true);
		f.write(allPieces);
		f.close();
		std::cout << "Done: " << downloadDir << std::endl;
	}
	else {
		throw Exception("Error in torrent file, no length or files");
	}
}

size_t Pieces::downloaded() {
	size_t dowloaded = 0;
	for (int i = 0; i < pieces.size(); i++) {
		for (int j = 0; j < pieces[i].size(); j++) {
			dowloaded += pieces[i][j].size();
		}
	}
	return dowloaded;
}

Handle Pieces::createLock() const {
	const auto fileHandle = CreateMutexA(
		NULL,
		FALSE,
		"Hello");
	return Handle(fileHandle);
}
