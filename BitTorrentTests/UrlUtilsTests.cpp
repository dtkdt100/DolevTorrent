#include "pch.h"
#include "BitTorrent/UrlUtils.h"

// // We will test here the class UrlUtils
TEST(UrlUtilsTest, TestDomainName) {
	std::string url = "udp://tracker.openbittorrent.com:80";
	std::string domain = UrlUtils::getDomain(url);
	EXPECT_EQ(domain, "tracker.openbittorrent.com");
}

TEST(UrlUtilsTest, TestPort) {
	std::string url = "udp://tracker.openbittorrent.com:80";
	int port = UrlUtils::getPort(url);
	EXPECT_EQ(port, 80);
}