#pragma once
#include <iostream>

class UrlUtils {
public:
	static std::string getDomain(std::string& url);
	static int getPort(std::string& url);
};

