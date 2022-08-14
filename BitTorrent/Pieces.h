#pragma once
#include "Exception.h"
#include "BenCodeObject.h"
#include "TorrentParser.h"
#include "Directory.h"

const std::string DOWNLOADPATH = "C:\\temp";

struct PieceInfo {
	int index;
	int begin;
	int len;

	PieceInfo(int index, int begin, int len) {
		this->index = index;
		this->begin = begin;
		this->len = len;
	}

	PieceInfo(){}
};

class Pieces {
public:
	Pieces(BenCodeObject& torrent);
	
	void addPiece(PieceInfo piece, Buffer& block);
	void checkPiece(int index);
	PieceInfo pieceNeeded();
	void failedPiece(PieceInfo info);
	bool onDone();
	void printPerstage();
	void downloadFilesToDisk();

	BenCodeObject& torrent;
private:
	size_t downloaded();

	std::vector<std::vector<Buffer>> pieces;
	std::vector<std::vector<bool>> requested;
	bool startDownloadedToDisk = false;
	Handle createLock() const;
	Handle mutexHandle;
};

