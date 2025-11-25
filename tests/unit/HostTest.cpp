#include <gtest/gtest.h>

#include <testcontainers/core/Host.hpp>
#include <testcontainers/Error.hpp>

using namespace testcontainers;

// ====================
// Host::Addr Tests
// ====================

TEST(HostTest, AddrIpv4) {
  auto host = Host::Addr("192.168.1.1");
  EXPECT_TRUE(host.is_valid());
}

TEST(HostTest, AddrIpv6) {
  auto host = Host::Addr("2001:db8::1");
  EXPECT_TRUE(host.is_valid());
}

TEST(HostTest, AddrLocalhost) {
  auto host = Host::Addr("127.0.0.1");
  EXPECT_TRUE(host.is_valid());
}

TEST(HostTest, AddrEmpty) {
  EXPECT_THROW(Host::Addr(""), Error);
}

TEST(HostTest, AddrDomain) {
  EXPECT_THROW(Host::Addr("database.local"), Error);
}

// ====================
// Host::HostGatewayLinux Tests
// ====================

TEST(HostTest, HostGatewayLinux) {
  auto host = Host::HostGatewayLinux();
  EXPECT_TRUE(host.is_valid());
}

// ====================
// Host Move Semantics Tests
// ====================

TEST(HostTest, MoveConstructor) {
  auto host1 = Host::Addr("10.0.0.1");
  auto host2 = std::move(host1);
  EXPECT_TRUE(host2.is_valid());
}

TEST(HostTest, MoveAssignment) {
  auto host1 = Host::Addr("10.0.0.1");
  auto host2 = Host::Addr("10.0.0.2");
  host2 = std::move(host1);
  EXPECT_TRUE(host2.is_valid());
}

// ====================
// Host Edge Cases Tests
// ====================

TEST(HostTest, AddrWithPort) {
  EXPECT_THROW(Host::Addr("192.168.1.1:8080"), Error);
}

TEST(HostTest, AddrVeryLongDomain) {
  std::string long_domain = std::string(200, 'a') + ".local";
  EXPECT_THROW(Host::Addr(long_domain), Error);
}

TEST(HostTest, AddrIpv4AllZeros) {
  auto host = Host::Addr("0.0.0.0");
  EXPECT_TRUE(host.is_valid());
}

TEST(HostTest, AddrIpv4Broadcast) {
  auto host = Host::Addr("255.255.255.255");
  EXPECT_TRUE(host.is_valid());
}

TEST(HostTest, AddrIpv6Localhost) {
  auto host = Host::Addr("::1");
  EXPECT_TRUE(host.is_valid());
}

TEST(HostTest, AddrIpv6Unspecified) {
  auto host = Host::Addr("::");
  EXPECT_TRUE(host.is_valid());
}

TEST(HostTest, AddrPrivateRange10) {
  auto host = Host::Addr("10.1.2.3");
  EXPECT_TRUE(host.is_valid());
}

TEST(HostTest, AddrPrivateRange172) {
  auto host = Host::Addr("172.16.0.1");
  EXPECT_TRUE(host.is_valid());
}

TEST(HostTest, AddrPrivateRange192) {
  auto host = Host::Addr("192.168.0.1");
  EXPECT_TRUE(host.is_valid());
}

