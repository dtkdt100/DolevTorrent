#include "Socket.h"

Socket::Socket(int sType, int sProtocol): sType(sType), sProtocol(sProtocol) {
	if ((sock = socket(AF_INET, sType, sProtocol)) == INVALID_SOCKET) {
		throw Exception("socket failed");
		closeSocket();
	}
}

Socket::~Socket() {
	try {
		closeSocket();
	}
	catch (...) {
		
	}
}

void Socket::connectSocket(std::string& hostname, int port) {
	struct addrinfo hints = {}, * addrs;


	hints.ai_family = AF_INET;

	
	hints.ai_socktype =	sType;
	hints.ai_protocol = sProtocol;

	if (getaddrinfo(hostname.c_str(), std::to_string(port).c_str(), &hints, &addrs) != 0) {
		throw Exception("getaddrinfo failed");
	}


	if (connect(sock, addrs->ai_addr, addrs->ai_addrlen) != 0) {
		throw Exception("connected failed");
	}
	freeaddrinfo(addrs);
}

void Socket::sendMessage(std::string& message) {
	if (send(sock, message.c_str(), message.length(), 0) == SOCKET_ERROR) {
		throw Exception("Error in send");
	}
}

std::string Socket::receiveMessage(int bytesToReceive) {
	char* buffer = new char[bytesToReceive];
	int bytesReceived = recv(sock, buffer, bytesToReceive, 0);
	if (bytesReceived == SOCKET_ERROR) {
		throw Exception("Error in recv");
	}
	std::string retVal = "";
	for (int i = 0; i < bytesReceived; i++) {
		retVal += (buffer)[i];
	}
	delete[] buffer;
	return retVal;
}

static int msgLen(bool handshake, Buffer* buf) {
	if (handshake) {
		return 19 + 49;
	}
	else {
		// read first 4 bytes in buf
		int len = (int)(*buf)[0] << 24 | (int)(*buf)[1] << 16 | (int)(*buf)[2] << 8 | (int)(*buf)[3];

		return len + 4;
	}
}

std::string Socket::onWholeMessage(bool handshake) {
	Buffer buf;

	int bytesToReceive = 4;
	while (true) {
		std::string message = receiveMessage(bytesToReceive);
		for (auto c : message) {
			buf.push_back(c);
		}
		bytesToReceive = msgLen(handshake, &buf) - 4;
		if (buf.size() >= msgLen(handshake, &buf)) {
			return std::string(buf.begin(), buf.end());
		} 
		
	}
	return "";
}

void Socket::closeSocket() {
	if (closesocket(sock) != 0) {
		throw Exception("closesocket failed");
	}
}
