#include "UrlUtils.h"

std::string UrlUtils::getDomain(std::string& url) {
	std::string domain;
	size_t pos = url.find("://");
	if (pos != std::string::npos) {
		domain = url.substr(pos + 3);
	}
	pos = domain.find("/");
	if (pos != std::string::npos) {
		domain = domain.substr(0, pos);
	}
	pos = domain.find(":");
	if (pos != std::string::npos) {
		domain = domain.substr(0, pos);
	}
	return domain;
	
}

int UrlUtils::getPort(std::string& url) {
	size_t found = url.find_last_of(":");
	size_t found1 = url.find_first_of("/");
	std::string port = url.substr(found + 1, found1 - found - 1);
	return std::atoi(port.c_str());
}

Protocol UrlUtils::getProtocol(std::string& url) {
	std::string protocol;
	size_t pos = url.find("://");
	if (pos != std::string::npos) {
		protocol = url.substr(0, pos);
	}
	if (protocol == "http") {
		return Protocol::HTTP;
	}
	else if (protocol == "https") {
		return Protocol::HTTPS;
	}
	else if (protocol == "udp") {
		return Protocol::UDP;
	}
	else if (protocol == "wss") {
		return Protocol::WSS;
	}
	else {
		return Protocol::UNKNOWN;
	}
}
