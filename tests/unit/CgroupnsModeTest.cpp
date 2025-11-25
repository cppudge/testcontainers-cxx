#include <gtest/gtest.h>

#include <testcontainers/core/CgroupnsMode.hpp>

using namespace testcontainers;

// ====================
// CgroupnsMode::Private Tests
// ====================

TEST(CgroupnsModeTest, PrivateBasic) {
  auto mode = CgroupnsMode::Private();
  EXPECT_TRUE(mode.is_valid());
}

// ====================
// CgroupnsMode::Host Tests
// ====================

TEST(CgroupnsModeTest, HostBasic) {
  auto mode = CgroupnsMode::Host();
  EXPECT_TRUE(mode.is_valid());
}

// ====================
// CgroupnsMode Move Semantics Tests
// ====================

TEST(CgroupnsModeTest, MoveConstructorPrivate) {
  auto mode1 = CgroupnsMode::Private();
  auto mode2 = std::move(mode1);
  EXPECT_TRUE(mode2.is_valid());
}

TEST(CgroupnsModeTest, MoveConstructorHost) {
  auto mode1 = CgroupnsMode::Host();
  auto mode2 = std::move(mode1);
  EXPECT_TRUE(mode2.is_valid());
}

TEST(CgroupnsModeTest, MoveAssignment) {
  auto mode1 = CgroupnsMode::Private();
  auto mode2 = CgroupnsMode::Host();
  mode2 = std::move(mode1);
  EXPECT_TRUE(mode2.is_valid());
}

// ====================
// CgroupnsMode Usage Tests
// ====================

TEST(CgroupnsModeTest, CanCreateMultiplePrivate) {
  auto mode1 = CgroupnsMode::Private();
  auto mode2 = CgroupnsMode::Private();
  EXPECT_TRUE(mode1.is_valid());
  EXPECT_TRUE(mode2.is_valid());
}

TEST(CgroupnsModeTest, CanCreateMultipleHost) {
  auto mode1 = CgroupnsMode::Host();
  auto mode2 = CgroupnsMode::Host();
  EXPECT_TRUE(mode1.is_valid());
  EXPECT_TRUE(mode2.is_valid());
}

