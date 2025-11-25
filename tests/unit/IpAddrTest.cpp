#include <gtest/gtest.h>

#include <testcontainers/system/ip/IpAddr.hpp>
#include <testcontainers/system/ip/Ipv4Addr.hpp>
#include <testcontainers/system/ip/Ipv6Addr.hpp>

using namespace testcontainers;

// ====================
// IpAddr from IPv4 Tests
// ====================

TEST(IpAddrTest, FromIpv4Localhost) {
  auto ipv4 = Ipv4Addr::localhost();
  auto ip = IpAddr::from_ipv4(std::move(ipv4));
  EXPECT_TRUE(ip.is_valid());
  EXPECT_TRUE(ip.is_ipv4());
  EXPECT_FALSE(ip.is_ipv6());
  EXPECT_TRUE(ip.is_loopback());
}

TEST(IpAddrTest, FromIpv4Unspecified) {
  auto ipv4 = Ipv4Addr::unspecified();
  auto ip = IpAddr::from_ipv4(std::move(ipv4));
  EXPECT_TRUE(ip.is_valid());
  EXPECT_TRUE(ip.is_unspecified());
}

TEST(IpAddrTest, FromIpv4Custom) {
  auto ipv4 = Ipv4Addr(192, 168, 1, 1);
  auto ip = IpAddr::from_ipv4(std::move(ipv4));
  EXPECT_TRUE(ip.is_valid());
  EXPECT_TRUE(ip.is_ipv4());
  EXPECT_EQ(ip.to_string(), "192.168.1.1");
}

// ====================
// IpAddr from IPv6 Tests
// ====================

TEST(IpAddrTest, FromIpv6Localhost) {
  auto ipv6 = Ipv6Addr::localhost();
  auto ip = IpAddr::from_ipv6(std::move(ipv6));
  EXPECT_TRUE(ip.is_valid());
  EXPECT_TRUE(ip.is_ipv6());
  EXPECT_FALSE(ip.is_ipv4());
  EXPECT_TRUE(ip.is_loopback());
}

TEST(IpAddrTest, FromIpv6Unspecified) {
  auto ipv6 = Ipv6Addr::unspecified();
  auto ip = IpAddr::from_ipv6(std::move(ipv6));
  EXPECT_TRUE(ip.is_valid());
  EXPECT_TRUE(ip.is_unspecified());
}

TEST(IpAddrTest, FromIpv6Custom) {
  auto ipv6 = Ipv6Addr(0x2001, 0xdb8, 0, 0, 0, 0, 0, 1);
  auto ip = IpAddr::from_ipv6(std::move(ipv6));
  EXPECT_TRUE(ip.is_valid());
  EXPECT_TRUE(ip.is_ipv6());
}

// ====================
// IpAddr to_canonical Tests
// ====================

TEST(IpAddrTest, ToCanonicalIpv4) {
  auto ipv4 = Ipv4Addr(10, 0, 0, 1);
  auto ip = IpAddr::from_ipv4(std::move(ipv4));
  auto canonical = ip.to_canonical();
  EXPECT_TRUE(canonical.is_valid());
}

TEST(IpAddrTest, ToCanonicalIpv6) {
  auto ipv6 = Ipv6Addr::localhost();
  auto ip = IpAddr::from_ipv6(std::move(ipv6));
  auto canonical = ip.to_canonical();
  EXPECT_TRUE(canonical.is_valid());
}

// ====================
// IpAddr to_ipv4/to_ipv6 Tests
// ====================

TEST(IpAddrTest, ToIpv4) {
  auto ipv4_orig = Ipv4Addr(192, 168, 0, 1);
  auto ip = IpAddr::from_ipv4(std::move(ipv4_orig));
  auto ipv4_back = ip.to_ipv4();
  EXPECT_TRUE(ipv4_back.is_valid());
  EXPECT_EQ(ipv4_back.to_string(), "192.168.0.1");
}

TEST(IpAddrTest, ToIpv6) {
  auto ipv6_orig = Ipv6Addr(0x2001, 0xdb8, 0, 0, 0, 0, 0, 1);
  auto ip = IpAddr::from_ipv6(std::move(ipv6_orig));
  auto ipv6_back = ip.to_ipv6();
  EXPECT_TRUE(ipv6_back.is_valid());
}

// ====================
// IpAddr to_string Tests
// ====================

TEST(IpAddrTest, ToStringIpv4) {
  auto ipv4 = Ipv4Addr(127, 0, 0, 1);
  auto ip = IpAddr::from_ipv4(std::move(ipv4));
  EXPECT_EQ(ip.to_string(), "127.0.0.1");
}

TEST(IpAddrTest, ToStringIpv6) {
  auto ipv6 = Ipv6Addr::localhost();
  auto ip = IpAddr::from_ipv6(std::move(ipv6));
  EXPECT_EQ(ip.to_string(), "::1");
}

// ====================
// IpAddr Move Semantics Tests
// ====================

TEST(IpAddrTest, MoveConstructor) {
  auto ipv4 = Ipv4Addr::localhost();
  auto ip1 = IpAddr::from_ipv4(std::move(ipv4));
  auto ip2 = std::move(ip1);
  EXPECT_TRUE(ip2.is_valid());
}

TEST(IpAddrTest, MoveAssignment) {
  auto ip1 = IpAddr::from_ipv4(Ipv4Addr::localhost());
  auto ip2 = IpAddr::from_ipv4(Ipv4Addr::broadcast());
  ip2 = std::move(ip1);
  EXPECT_TRUE(ip2.is_valid());
}

