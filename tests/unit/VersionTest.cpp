#include <gtest/gtest.h>
#include <testcontainers/Version.hpp>

TEST(VersionTest, ReturnsNonEmptyString) {
  auto v = testcontainers::version();
  ASSERT_FALSE(v.empty());
}

TEST(VersionTest, ReturnsExpectedVersion) {
  auto v = testcontainers::version();
  // Should return "0.1.0" as defined in rust/Cargo.toml
  EXPECT_EQ(v, "0.1.0");
}
