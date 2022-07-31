#include "Socket.h"

Socket::Socket(int sType, int sProtocol) {
	if ((sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == INVALID_SOCKET) {
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
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_UDP;

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

std::string Socket::receiveMessage() {
	char buffer[1024];
	int bytesReceived = recv(sock, buffer, 1024, 0);
	if (bytesReceived == SOCKET_ERROR) {
		throw Exception("Error in recv");
	}
	std::string retVal = "";
	for (int i = 0; i < bytesReceived; i++) {
		retVal += buffer[i];
	}
	return retVal;
}

void Socket::closeSocket() {
	if (closesocket(sock) != 0) {
		throw Exception("closesocket failed");
	}
}
