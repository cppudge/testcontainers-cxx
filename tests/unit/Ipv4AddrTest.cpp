#include <array>

#include <gtest/gtest.h>

#include <testcontainers/system/ip/Ipv4Addr.hpp>

using namespace testcontainers;

// ====================
// Ipv4Addr Constructor Tests
// ====================

TEST(Ipv4AddrTest, ConstructorBasic) {
  Ipv4Addr addr(127, 0, 0, 1);
  EXPECT_TRUE(addr.is_valid());
  EXPECT_EQ(addr.to_string(), "127.0.0.1");
}

TEST(Ipv4AddrTest, ConstructorAllZeros) {
  Ipv4Addr addr(0, 0, 0, 0);
  EXPECT_TRUE(addr.is_valid());
  EXPECT_TRUE(addr.is_unspecified());
}

TEST(Ipv4AddrTest, ConstructorAllOnes) {
  Ipv4Addr addr(255, 255, 255, 255);
  EXPECT_TRUE(addr.is_valid());
  EXPECT_TRUE(addr.is_broadcast());
}

// ====================
// Ipv4Addr Static Factory Tests
// ====================

TEST(Ipv4AddrTest, Localhost) {
  auto addr = Ipv4Addr::localhost();
  EXPECT_TRUE(addr.is_valid());
  EXPECT_TRUE(addr.is_loopback());
  EXPECT_EQ(addr.to_string(), "127.0.0.1");
}

TEST(Ipv4AddrTest, Unspecified) {
  auto addr = Ipv4Addr::unspecified();
  EXPECT_TRUE(addr.is_valid());
  EXPECT_TRUE(addr.is_unspecified());
  EXPECT_EQ(addr.to_string(), "0.0.0.0");
}

TEST(Ipv4AddrTest, Broadcast) {
  auto addr = Ipv4Addr::broadcast();
  EXPECT_TRUE(addr.is_valid());
  EXPECT_TRUE(addr.is_broadcast());
  EXPECT_EQ(addr.to_string(), "255.255.255.255");
}

TEST(Ipv4AddrTest, FromBits) {
  auto addr = Ipv4Addr::from_bits(0x7F000001); // 127.0.0.1
  EXPECT_TRUE(addr.is_valid());
  EXPECT_EQ(addr.to_string(), "127.0.0.1");
}

TEST(Ipv4AddrTest, FromOctets) {
  std::array<std::uint8_t, 4> octets = {192, 168, 1, 1};
  auto addr = Ipv4Addr::from_octets(octets);
  EXPECT_TRUE(addr.is_valid());
  EXPECT_EQ(addr.to_string(), "192.168.1.1");
}

// ====================
// Ipv4Addr to_bits Tests
// ====================

TEST(Ipv4AddrTest, ToBits) {
  Ipv4Addr addr(127, 0, 0, 1);
  EXPECT_EQ(addr.to_bits(), 0x7F000001);
}

TEST(Ipv4AddrTest, ToBitsZero) {
  auto addr = Ipv4Addr::unspecified();
  EXPECT_EQ(addr.to_bits(), 0);
}

TEST(Ipv4AddrTest, ToBitsBroadcast) {
  auto addr = Ipv4Addr::broadcast();
  EXPECT_EQ(addr.to_bits(), 0xFFFFFFFF);
}

// ====================
// Ipv4Addr octets Tests
// ====================

TEST(Ipv4AddrTest, Octets) {
  Ipv4Addr addr(192, 168, 1, 1);
  auto octets = addr.octets();
  EXPECT_EQ(octets[0], 192);
  EXPECT_EQ(octets[1], 168);
  EXPECT_EQ(octets[2], 1);
  EXPECT_EQ(octets[3], 1);
}

// ====================
// Ipv4Addr is_* Tests
// ====================

TEST(Ipv4AddrTest, IsLoopback) {
  EXPECT_TRUE(Ipv4Addr::localhost().is_loopback());
  EXPECT_TRUE(Ipv4Addr(127, 0, 0, 2).is_loopback());
  EXPECT_FALSE(Ipv4Addr(192, 168, 1, 1).is_loopback());
}

TEST(Ipv4AddrTest, IsPrivate) {
  EXPECT_TRUE(Ipv4Addr(10, 0, 0, 1).is_private());
  EXPECT_TRUE(Ipv4Addr(172, 16, 0, 1).is_private());
  EXPECT_TRUE(Ipv4Addr(192, 168, 1, 1).is_private());
  EXPECT_FALSE(Ipv4Addr(8, 8, 8, 8).is_private());
}

TEST(Ipv4AddrTest, IsLinkLocal) {
  EXPECT_TRUE(Ipv4Addr(169, 254, 0, 1).is_link_local());
  EXPECT_FALSE(Ipv4Addr(192, 168, 1, 1).is_link_local());
}

TEST(Ipv4AddrTest, IsMulticast) {
  EXPECT_TRUE(Ipv4Addr(224, 0, 0, 1).is_multicast());
  EXPECT_TRUE(Ipv4Addr(239, 255, 255, 255).is_multicast());
  EXPECT_FALSE(Ipv4Addr(192, 168, 1, 1).is_multicast());
}

TEST(Ipv4AddrTest, IsDocumentation) {
  EXPECT_TRUE(Ipv4Addr(192, 0, 2, 1).is_documentation());
  EXPECT_TRUE(Ipv4Addr(198, 51, 100, 1).is_documentation());
  EXPECT_TRUE(Ipv4Addr(203, 0, 113, 1).is_documentation());
  EXPECT_FALSE(Ipv4Addr(192, 168, 1, 1).is_documentation());
}

// ====================
// Ipv4Addr to_string Tests
// ====================

TEST(Ipv4AddrTest, ToString) {
  EXPECT_EQ(Ipv4Addr(127, 0, 0, 1).to_string(), "127.0.0.1");
  EXPECT_EQ(Ipv4Addr(192, 168, 1, 1).to_string(), "192.168.1.1");
  EXPECT_EQ(Ipv4Addr(0, 0, 0, 0).to_string(), "0.0.0.0");
  EXPECT_EQ(Ipv4Addr(255, 255, 255, 255).to_string(), "255.255.255.255");
}

// ====================
// Ipv4Addr Move Semantics Tests
// ====================

TEST(Ipv4AddrTest, MoveConstructor) {
  Ipv4Addr addr1(192, 168, 1, 1);
  Ipv4Addr addr2(std::move(addr1));
  EXPECT_TRUE(addr2.is_valid());
}

TEST(Ipv4AddrTest, MoveAssignment) {
  Ipv4Addr addr1(192, 168, 1, 1);
  Ipv4Addr addr2(10, 0, 0, 1);
  addr2 = std::move(addr1);
  EXPECT_TRUE(addr2.is_valid());
}

// ====================
// Ipv4Addr Edge Cases Tests
// ====================

TEST(Ipv4AddrTest, BITS_Constant) {
  EXPECT_EQ(Ipv4Addr::BITS, 32);
}

TEST(Ipv4AddrTest, RoundTripThroughBits) {
  Ipv4Addr original(192, 168, 1, 100);
  auto bits = original.to_bits();
  auto restored = Ipv4Addr::from_bits(bits);
  EXPECT_EQ(original.to_string(), restored.to_string());
}

TEST(Ipv4AddrTest, RoundTripThroughOctets) {
  Ipv4Addr original(172, 16, 254, 1);
  auto octets = original.octets();
  auto restored = Ipv4Addr::from_octets(octets);
  EXPECT_EQ(original.to_string(), restored.to_string());
}

