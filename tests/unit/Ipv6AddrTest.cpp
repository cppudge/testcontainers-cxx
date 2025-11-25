#include <array>

#include <gtest/gtest.h>

#include <testcontainers/system/ip/Ipv6Addr.hpp>
#include <testcontainers/system/ip/IpAddr.hpp>

using namespace testcontainers;

// ====================
// Ipv6Addr Constructor Tests
// ====================

TEST(Ipv6AddrTest, ConstructorBasic) {
  Ipv6Addr addr(0x2001, 0xdb8, 0, 0, 0, 0, 0, 1);
  EXPECT_TRUE(addr.is_valid());
}

TEST(Ipv6AddrTest, ConstructorAllZeros) {
  Ipv6Addr addr(0, 0, 0, 0, 0, 0, 0, 0);
  EXPECT_TRUE(addr.is_valid());
  EXPECT_TRUE(addr.is_unspecified());
}

TEST(Ipv6AddrTest, ConstructorAllOnes) {
  Ipv6Addr addr(0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF);
  EXPECT_TRUE(addr.is_valid());
}

// ====================
// Ipv6Addr Static Factory Tests
// ====================

TEST(Ipv6AddrTest, Localhost) {
  auto addr = Ipv6Addr::localhost();
  EXPECT_TRUE(addr.is_valid());
  EXPECT_TRUE(addr.is_loopback());
  EXPECT_EQ(addr.to_string(), "::1");
}

TEST(Ipv6AddrTest, Unspecified) {
  auto addr = Ipv6Addr::unspecified();
  EXPECT_TRUE(addr.is_valid());
  EXPECT_TRUE(addr.is_unspecified());
  EXPECT_EQ(addr.to_string(), "::");
}

TEST(Ipv6AddrTest, FromBits) {
  auto addr = Ipv6Addr::from_bits(0x0000000000000001, 0x0000000000000000); // ::1
  EXPECT_TRUE(addr.is_valid());
  EXPECT_FALSE(addr.is_loopback());
}

TEST(Ipv6AddrTest, FromSegments) {
  std::array<std::uint16_t, 8> segments = {0x2001, 0xdb8, 0, 0, 0, 0, 0, 1};
  auto addr = Ipv6Addr::from_segments(segments);
  EXPECT_TRUE(addr.is_valid());
}

TEST(Ipv6AddrTest, FromOctets) {
  std::array<std::uint8_t, 16> octets = {0x20, 0x01, 0x0d, 0xb8, 0, 0, 0, 0,
                                          0,    0,    0,    0,    0, 0, 0, 1};
  auto addr = Ipv6Addr::from_octets(octets);
  EXPECT_TRUE(addr.is_valid());
}

// ====================
// Ipv6Addr to_bits Tests
// ====================

TEST(Ipv6AddrTest, ToBits) {
  auto addr = Ipv6Addr::localhost();
  auto [low, high] = addr.to_bits();
  EXPECT_EQ(low, 0x0000000000000000ULL);
  EXPECT_EQ(high, 0x0000000000000001ULL);
}

TEST(Ipv6AddrTest, ToBitsUnspecified) {
  auto addr = Ipv6Addr::unspecified();
  auto [low, high] = addr.to_bits();
  EXPECT_EQ(low, 0ULL);
  EXPECT_EQ(high, 0ULL);
}

// ====================
// Ipv6Addr segments Tests
// ====================

TEST(Ipv6AddrTest, Segments) {
  Ipv6Addr addr(0x2001, 0xdb8, 0, 0, 0, 0, 0, 1);
  auto segments = addr.segments();
  EXPECT_EQ(segments[0], 0x2001);
  EXPECT_EQ(segments[1], 0xdb8);
  EXPECT_EQ(segments[7], 1);
}

TEST(Ipv6AddrTest, SegmentsLocalhost) {
  auto addr = Ipv6Addr::localhost();
  auto segments = addr.segments();
  for (int i = 0; i < 7; ++i) {
    EXPECT_EQ(segments[i], 0);
  }
  EXPECT_EQ(segments[7], 1);
}

// ====================
// Ipv6Addr octets Tests
// ====================

TEST(Ipv6AddrTest, Octets) {
  Ipv6Addr addr(0x2001, 0xdb8, 0, 0, 0, 0, 0, 1);
  auto octets = addr.octets();
  EXPECT_EQ(octets[0], 0x20);
  EXPECT_EQ(octets[1], 0x01);
  EXPECT_EQ(octets[2], 0x0d);
  EXPECT_EQ(octets[3], 0xb8);
  EXPECT_EQ(octets[15], 0x01);
}

// ====================
// Ipv6Addr is_* Tests
// ====================

TEST(Ipv6AddrTest, IsLoopback) {
  EXPECT_TRUE(Ipv6Addr::localhost().is_loopback());
  EXPECT_FALSE(Ipv6Addr::unspecified().is_loopback());
  EXPECT_FALSE(Ipv6Addr(0x2001, 0xdb8, 0, 0, 0, 0, 0, 1).is_loopback());
}

TEST(Ipv6AddrTest, IsUnicastLinkLocal) {
  Ipv6Addr addr(0xfe80, 0, 0, 0, 0, 0, 0, 1);
  EXPECT_TRUE(addr.is_unicast_link_local());
  EXPECT_FALSE(Ipv6Addr::localhost().is_unicast_link_local());
}

TEST(Ipv6AddrTest, IsUniqueLocal) {
  Ipv6Addr addr(0xfc00, 0, 0, 0, 0, 0, 0, 1);
  EXPECT_TRUE(addr.is_unique_local());
  EXPECT_FALSE(Ipv6Addr::localhost().is_unique_local());
}

TEST(Ipv6AddrTest, IsMulticast) {
  Ipv6Addr addr(0xff02, 0, 0, 0, 0, 0, 0, 1);
  EXPECT_TRUE(addr.is_multicast());
  EXPECT_FALSE(Ipv6Addr::localhost().is_multicast());
}

// ====================
// Ipv6Addr to_canonical Tests
// ====================

TEST(Ipv6AddrTest, ToCanonical) {
  auto addr = Ipv6Addr(0x2001, 0xdb8, 0, 0, 0, 0, 0, 1);
  auto canonical = addr.to_canonical();
  EXPECT_TRUE(canonical.is_valid());
}

TEST(Ipv6AddrTest, ToCanonicalLocalhost) {
  auto addr = Ipv6Addr::localhost();
  auto canonical = addr.to_canonical();
  EXPECT_TRUE(canonical.is_valid());
}

// ====================
// Ipv6Addr to_string Tests
// ====================

TEST(Ipv6AddrTest, ToString) {
  EXPECT_EQ(Ipv6Addr::localhost().to_string(), "::1");
  EXPECT_EQ(Ipv6Addr::unspecified().to_string(), "::");
}

TEST(Ipv6AddrTest, ToStringCustom) {
  Ipv6Addr addr(0x2001, 0xdb8, 0, 0, 0, 0, 0, 1);
  auto str = addr.to_string();
  EXPECT_FALSE(str.empty());
}

// ====================
// Ipv6Addr Move Semantics Tests
// ====================

TEST(Ipv6AddrTest, MoveConstructor) {
  Ipv6Addr addr1(0x2001, 0xdb8, 0, 0, 0, 0, 0, 1);
  Ipv6Addr addr2(std::move(addr1));
  EXPECT_TRUE(addr2.is_valid());
}

TEST(Ipv6AddrTest, MoveAssignment) {
  Ipv6Addr addr1(0x2001, 0xdb8, 0, 0, 0, 0, 0, 1);
  Ipv6Addr addr2 = Ipv6Addr::localhost();
  addr2 = std::move(addr1);
  EXPECT_TRUE(addr2.is_valid());
}

// ====================
// Ipv6Addr Edge Cases Tests
// ====================

TEST(Ipv6AddrTest, BITS_Constant) {
  EXPECT_EQ(Ipv6Addr::BITS, 128);
}

TEST(Ipv6AddrTest, RoundTripThroughBits) {
  Ipv6Addr original(0x2001, 0xdb8, 0, 0, 0, 0, 0, 1);
  auto [low, high] = original.to_bits();
  auto restored = Ipv6Addr::from_bits(low, high);
  EXPECT_EQ(original.to_string(), restored.to_string());
}

TEST(Ipv6AddrTest, RoundTripThroughSegments) {
  Ipv6Addr original(0x2001, 0xdb8, 0xabcd, 0x1234, 0x5678, 0x9abc, 0xdef0, 0x1234);
  auto segments = original.segments();
  auto restored = Ipv6Addr::from_segments(segments);
  EXPECT_EQ(original.to_string(), restored.to_string());
}

TEST(Ipv6AddrTest, RoundTripThroughOctets) {
  Ipv6Addr original(0x2001, 0xdb8, 0, 0, 0, 0, 0, 1);
  auto octets = original.octets();
  auto restored = Ipv6Addr::from_octets(octets);
  EXPECT_EQ(original.to_string(), restored.to_string());
}

