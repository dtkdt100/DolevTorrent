#pragma once
#include <iostream>

enum Protocol {
	WSS,
	UDP,
	HTTPS,
	HTTP,
	UNKNOWN
};

class UrlUtils {
public:
	static std::string getDomain(std::string& url);
	static int getPort(std::string& url);
	static Protocol getProtocol(std::string& url);
};

