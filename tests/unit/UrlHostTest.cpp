#include <gtest/gtest.h>

#include <testcontainers/system/UrlHost.hpp>
#include <testcontainers/system/ip/Ipv4Addr.hpp>
#include <testcontainers/system/ip/Ipv6Addr.hpp>

using namespace testcontainers;

// ====================
// UrlHost domain Tests
// ====================

TEST(UrlHostTest, DomainSimple) {
  auto host = UrlHost::domain("example.com");
  EXPECT_TRUE(host.is_domain());
  EXPECT_FALSE(host.is_ipv4());
  EXPECT_FALSE(host.is_ipv6());
}

TEST(UrlHostTest, DomainSubdomain) {
  auto host = UrlHost::domain("api.example.com");
  EXPECT_TRUE(host.is_domain());
}

TEST(UrlHostTest, DomainLocalhost) {
  auto host = UrlHost::domain("localhost");
  EXPECT_TRUE(host.is_domain());
}

TEST(UrlHostTest, DomainEmpty) {
  auto host = UrlHost::domain("");
  EXPECT_TRUE(host.is_domain());
}

// ====================
// UrlHost from_ipv4 Tests
// ====================

TEST(UrlHostTest, FromIpv4Localhost) {
  auto ipv4 = Ipv4Addr::localhost();
  auto host = UrlHost::from_ipv4(std::move(ipv4));
  EXPECT_FALSE(host.is_domain());
  EXPECT_TRUE(host.is_ipv4());
  EXPECT_FALSE(host.is_ipv6());
}

TEST(UrlHostTest, FromIpv4Custom) {
  auto ipv4 = Ipv4Addr(192, 168, 1, 1);
  auto host = UrlHost::from_ipv4(std::move(ipv4));
  EXPECT_TRUE(host.is_ipv4());
}

// ====================
// UrlHost from_ipv6 Tests
// ====================

TEST(UrlHostTest, FromIpv6Localhost) {
  auto ipv6 = Ipv6Addr::localhost();
  auto host = UrlHost::from_ipv6(std::move(ipv6));
  EXPECT_FALSE(host.is_domain());
  EXPECT_FALSE(host.is_ipv4());
  EXPECT_TRUE(host.is_ipv6());
}

TEST(UrlHostTest, FromIpv6Custom) {
  auto ipv6 = Ipv6Addr(0x2001, 0xdb8, 0, 0, 0, 0, 0, 1);
  auto host = UrlHost::from_ipv6(std::move(ipv6));
  EXPECT_TRUE(host.is_ipv6());
}

// ====================
// UrlHost to_* Tests
// ====================

TEST(UrlHostTest, ToDomain) {
  auto host = UrlHost::domain("example.com");
  auto domain = host.to_domain();
  EXPECT_EQ(domain, "example.com");
}

TEST(UrlHostTest, ToIpv4) {
  auto ipv4_orig = Ipv4Addr(192, 168, 1, 1);
  auto host = UrlHost::from_ipv4(std::move(ipv4_orig));
  auto ipv4_back = host.to_ipv4();
  EXPECT_EQ(ipv4_back.to_string(), "192.168.1.1");
}

TEST(UrlHostTest, ToIpv6) {
  auto ipv6_orig = Ipv6Addr::localhost();
  auto host = UrlHost::from_ipv6(std::move(ipv6_orig));
  auto ipv6_back = host.to_ipv6();
  EXPECT_EQ(ipv6_back.to_string(), "::1");
}

// ====================
// UrlHost to_string Tests
// ====================

TEST(UrlHostTest, ToStringDomain) {
  auto host = UrlHost::domain("example.com");
  EXPECT_EQ(host.to_string(), "example.com");
}

TEST(UrlHostTest, ToStringIpv4) {
  auto ipv4 = Ipv4Addr(127, 0, 0, 1);
  auto host = UrlHost::from_ipv4(std::move(ipv4));
  EXPECT_EQ(host.to_string(), "127.0.0.1");
}

TEST(UrlHostTest, ToStringIpv6) {
  auto ipv6 = Ipv6Addr::localhost();
  auto host = UrlHost::from_ipv6(std::move(ipv6));
  auto str = host.to_string();
  EXPECT_FALSE(str.empty());
}

// ====================
// UrlHost Move Semantics Tests
// ====================

TEST(UrlHostTest, MoveConstructor) {
  auto host1 = UrlHost::domain("example.com");
  auto host2 = std::move(host1);
  EXPECT_TRUE(host2.is_domain());
}

TEST(UrlHostTest, MoveAssignment) {
  auto host1 = UrlHost::domain("example.com");
  auto host2 = UrlHost::domain("other.com");
  host2 = std::move(host1);
  EXPECT_TRUE(host2.is_domain());
}

// ====================
// UrlHost Edge Cases Tests
// ====================

TEST(UrlHostTest, DomainWithPort) {
  auto host = UrlHost::domain("example.com:8080");
  EXPECT_TRUE(host.is_domain());
}

TEST(UrlHostTest, DomainWithDashes) {
  auto host = UrlHost::domain("my-domain-name.com");
  EXPECT_TRUE(host.is_domain());
}

TEST(UrlHostTest, DomainWithNumbers) {
  auto host = UrlHost::domain("example123.com");
  EXPECT_TRUE(host.is_domain());
}

TEST(UrlHostTest, VeryLongDomain) {
  std::string long_domain = std::string(200, 'a') + ".com";
  auto host = UrlHost::domain(long_domain);
  EXPECT_TRUE(host.is_domain());
}

