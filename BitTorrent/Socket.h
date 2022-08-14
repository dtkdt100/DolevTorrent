#pragma once
#pragma comment(lib, "Ws2_32.lib")
#include <ws2tcpip.h>
#include <Windows.h>
#include <iostream>
#include "Exception.h"
#include "WinShok.h"

class Socket {
public:
	Socket(int sType, int sProtocol);
	~Socket();
	void connectSocket(std::string& hostname, int port);
	void sendMessage(std::string& message);
	std::string receiveMessage(int bytesToReceive);
	std::string onWholeMessage(bool handshake);
	void closeSocket();
private:
	WinShok winShok;
	SOCKET sock;

	int sType;
	int sProtocol;
};

