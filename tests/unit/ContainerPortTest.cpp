#include <gtest/gtest.h>

#include <testcontainers/core/ContainerPort.hpp>

using namespace testcontainers;

// ====================
// ContainerPort::Tcp Tests
// ====================

TEST(ContainerPortTest, TcpBasic) {
  auto port = ContainerPort::Tcp(8080);
  EXPECT_TRUE(port.is_valid());
  EXPECT_TRUE(port.is_tcp());
  EXPECT_FALSE(port.is_udp());
  EXPECT_FALSE(port.is_sctp());
  EXPECT_EQ(port.as_u16(), 8080);
}

TEST(ContainerPortTest, TcpWellKnownPort) {
  auto port = ContainerPort::Tcp(80);
  EXPECT_TRUE(port.is_valid());
  EXPECT_EQ(port.as_u16(), 80);
}

TEST(ContainerPortTest, TcpHighPort) {
  auto port = ContainerPort::Tcp(65535);
  EXPECT_TRUE(port.is_valid());
  EXPECT_EQ(port.as_u16(), 65535);
}

TEST(ContainerPortTest, TcpZeroPort) {
  auto port = ContainerPort::Tcp(0);
  EXPECT_TRUE(port.is_valid());
  EXPECT_EQ(port.as_u16(), 0);
}

// ====================
// ContainerPort::Udp Tests
// ====================

TEST(ContainerPortTest, UdpBasic) {
  auto port = ContainerPort::Udp(53);
  EXPECT_TRUE(port.is_valid());
  EXPECT_FALSE(port.is_tcp());
  EXPECT_TRUE(port.is_udp());
  EXPECT_FALSE(port.is_sctp());
  EXPECT_EQ(port.as_u16(), 53);
}

TEST(ContainerPortTest, UdpDnsPort) {
  auto port = ContainerPort::Udp(53);
  EXPECT_EQ(port.as_u16(), 53);
}

TEST(ContainerPortTest, UdpNtpPort) {
  auto port = ContainerPort::Udp(123);
  EXPECT_EQ(port.as_u16(), 123);
}

// ====================
// ContainerPort::Sctp Tests
// ====================

TEST(ContainerPortTest, SctpBasic) {
  auto port = ContainerPort::Sctp(9899);
  EXPECT_TRUE(port.is_valid());
  EXPECT_FALSE(port.is_tcp());
  EXPECT_FALSE(port.is_udp());
  EXPECT_TRUE(port.is_sctp());
  EXPECT_EQ(port.as_u16(), 9899);
}

TEST(ContainerPortTest, SctpHighPort) {
  auto port = ContainerPort::Sctp(50000);
  EXPECT_TRUE(port.is_valid());
  EXPECT_EQ(port.as_u16(), 50000);
}

// ====================
// ContainerPort Getters Tests
// ====================

TEST(ContainerPortTest, AsU16Tcp) {
  auto port = ContainerPort::Tcp(12345);
  EXPECT_EQ(port.as_u16(), 12345);
}

TEST(ContainerPortTest, AsU16Udp) {
  auto port = ContainerPort::Udp(54321);
  EXPECT_EQ(port.as_u16(), 54321);
}

TEST(ContainerPortTest, AsU16Sctp) {
  auto port = ContainerPort::Sctp(9999);
  EXPECT_EQ(port.as_u16(), 9999);
}

// ====================
// ContainerPort Type Check Tests
// ====================

TEST(ContainerPortTest, TypeChecksTcp) {
  auto port = ContainerPort::Tcp(443);
  EXPECT_TRUE(port.is_tcp());
  EXPECT_FALSE(port.is_udp());
  EXPECT_FALSE(port.is_sctp());
}

TEST(ContainerPortTest, TypeChecksUdp) {
  auto port = ContainerPort::Udp(5353);
  EXPECT_FALSE(port.is_tcp());
  EXPECT_TRUE(port.is_udp());
  EXPECT_FALSE(port.is_sctp());
}

TEST(ContainerPortTest, TypeChecksSctp) {
  auto port = ContainerPort::Sctp(3868);
  EXPECT_FALSE(port.is_tcp());
  EXPECT_FALSE(port.is_udp());
  EXPECT_TRUE(port.is_sctp());
}

// ====================
// ContainerPort Move Semantics Tests
// ====================

TEST(ContainerPortTest, MoveConstructor) {
  auto port1 = ContainerPort::Tcp(8080);
  auto port2 = std::move(port1);
  EXPECT_TRUE(port2.is_valid());
  EXPECT_EQ(port2.as_u16(), 8080);
}

TEST(ContainerPortTest, MoveAssignment) {
  auto port1 = ContainerPort::Tcp(8080);
  auto port2 = ContainerPort::Tcp(9090);
  port2 = std::move(port1);
  EXPECT_TRUE(port2.is_valid());
  EXPECT_EQ(port2.as_u16(), 8080);
}

// ====================
// ContainerPort Edge Cases Tests
// ====================

TEST(ContainerPortTest, CommonHttpPorts) {
  auto http = ContainerPort::Tcp(80);
  auto https = ContainerPort::Tcp(443);
  auto alt_http = ContainerPort::Tcp(8080);
  auto alt_https = ContainerPort::Tcp(8443);

  EXPECT_EQ(http.as_u16(), 80);
  EXPECT_EQ(https.as_u16(), 443);
  EXPECT_EQ(alt_http.as_u16(), 8080);
  EXPECT_EQ(alt_https.as_u16(), 8443);
}

TEST(ContainerPortTest, CommonDatabasePorts) {
  auto postgres = ContainerPort::Tcp(5432);
  auto mysql = ContainerPort::Tcp(3306);
  auto redis = ContainerPort::Tcp(6379);
  auto mongo = ContainerPort::Tcp(27017);

  EXPECT_EQ(postgres.as_u16(), 5432);
  EXPECT_EQ(mysql.as_u16(), 3306);
  EXPECT_EQ(redis.as_u16(), 6379);
  EXPECT_EQ(mongo.as_u16(), 27017);
}

TEST(ContainerPortTest, EphemeralPortRange) {
  auto port1 = ContainerPort::Tcp(49152);
  auto port2 = ContainerPort::Tcp(65535);

  EXPECT_TRUE(port1.is_valid());
  EXPECT_TRUE(port2.is_valid());
}

