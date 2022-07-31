#include "WinShok.h"

WinShok::WinShok() {
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		throw Exception("WSAStartup failed");
	}
}

WinShok::~WinShok() {
	try {
		if (WSACleanup() != 0) {
			throw Exception("WSACleanup failed");
		}
	} 
	catch (...) {
		
	}
}
