#include <chrono>
#include <vector>

#include <gtest/gtest.h>

#include <testcontainers/core/Healthcheck.hpp>

using namespace testcontainers;

// ====================
// Healthcheck::none Tests
// ====================

TEST(HealthcheckTest, NoneBasic) {
  auto hc = Healthcheck::none();
  EXPECT_TRUE(hc.is_valid());
}

// ====================
// Healthcheck::cmd_shell Tests
// ====================

TEST(HealthcheckTest, CmdShellBasic) {
  auto hc = Healthcheck::cmd_shell("curl -f http://localhost/ || exit 1");
  EXPECT_TRUE(hc.is_valid());
}

TEST(HealthcheckTest, CmdShellSimple) {
  auto hc = Healthcheck::cmd_shell("test -f /ready");
  EXPECT_TRUE(hc.is_valid());
}

TEST(HealthcheckTest, CmdShellEmpty) {
  auto hc = Healthcheck::cmd_shell("");
  EXPECT_TRUE(hc.is_valid());
}

// ====================
// Healthcheck::cmd Tests
// ====================

TEST(HealthcheckTest, CmdBasic) {
  auto hc = Healthcheck::cmd({"curl", "-f", "http://localhost/"});
  EXPECT_TRUE(hc.is_valid());
}

TEST(HealthcheckTest, CmdSingleArg) {
  auto hc = Healthcheck::cmd({"healthcheck"});
  EXPECT_TRUE(hc.is_valid());
}

TEST(HealthcheckTest, CmdEmpty) {
  auto hc = Healthcheck::cmd({});
  EXPECT_TRUE(hc.is_valid());
}

// ====================
// Healthcheck::empty Tests
// ====================

TEST(HealthcheckTest, EmptyBasic) {
  auto hc = Healthcheck::empty();
  EXPECT_TRUE(hc.is_valid());
}

// ====================
// Healthcheck with_interval Tests
// ====================

TEST(HealthcheckTest, WithInterval) {
  auto hc = Healthcheck::cmd_shell("test -f /ready").with_interval(std::chrono::seconds(5));
  EXPECT_TRUE(hc.is_valid());
}

TEST(HealthcheckTest, WithIntervalMillis) {
  auto hc = Healthcheck::cmd_shell("test -f /ready").with_interval(std::chrono::milliseconds(500));
  EXPECT_TRUE(hc.is_valid());
}

TEST(HealthcheckTest, WithIntervalNullopt) {
  auto hc = Healthcheck::cmd_shell("test -f /ready").with_interval(std::nullopt);
  EXPECT_TRUE(hc.is_valid());
}

// ====================
// Healthcheck with_timeout Tests
// ====================

TEST(HealthcheckTest, WithTimeout) {
  auto hc = Healthcheck::cmd_shell("curl localhost").with_timeout(std::chrono::seconds(3));
  EXPECT_TRUE(hc.is_valid());
}

TEST(HealthcheckTest, WithTimeoutNullopt) {
  auto hc = Healthcheck::cmd_shell("test").with_timeout(std::nullopt);
  EXPECT_TRUE(hc.is_valid());
}

// ====================
// Healthcheck with_retries Tests
// ====================

TEST(HealthcheckTest, WithRetries) {
  auto hc = Healthcheck::cmd_shell("test").with_retries(3);
  EXPECT_TRUE(hc.is_valid());
}

TEST(HealthcheckTest, WithRetriesZero) {
  auto hc = Healthcheck::cmd_shell("test").with_retries(0);
  EXPECT_TRUE(hc.is_valid());
}

TEST(HealthcheckTest, WithRetriesNullopt) {
  auto hc = Healthcheck::cmd_shell("test").with_retries(std::nullopt);
  EXPECT_TRUE(hc.is_valid());
}

// ====================
// Healthcheck with_start_period Tests
// ====================

TEST(HealthcheckTest, WithStartPeriod) {
  auto hc = Healthcheck::cmd_shell("test").with_start_period(std::chrono::seconds(10));
  EXPECT_TRUE(hc.is_valid());
}

TEST(HealthcheckTest, WithStartPeriodNullopt) {
  auto hc = Healthcheck::cmd_shell("test").with_start_period(std::nullopt);
  EXPECT_TRUE(hc.is_valid());
}

// ====================
// Healthcheck with_start_interval Tests
// ====================

TEST(HealthcheckTest, WithStartInterval) {
  auto hc = Healthcheck::cmd_shell("test").with_start_interval(std::chrono::seconds(1));
  EXPECT_TRUE(hc.is_valid());
}

TEST(HealthcheckTest, WithStartIntervalNullopt) {
  auto hc = Healthcheck::cmd_shell("test").with_start_interval(std::nullopt);
  EXPECT_TRUE(hc.is_valid());
}

// ====================
// Healthcheck Fluent Chain Tests
// ====================

TEST(HealthcheckTest, FluentChainFull) {
  auto hc = Healthcheck::cmd_shell("curl -f http://localhost/health || exit 1")
                .with_interval(std::chrono::seconds(5))
                .with_timeout(std::chrono::seconds(3))
                .with_retries(3)
                .with_start_period(std::chrono::seconds(10))
                .with_start_interval(std::chrono::seconds(1));
  EXPECT_TRUE(hc.is_valid());
}

TEST(HealthcheckTest, FluentChainPartial) {
  auto hc = Healthcheck::cmd({"test", "-f", "/ready"})
                .with_interval(std::chrono::seconds(2))
                .with_retries(5);
  EXPECT_TRUE(hc.is_valid());
}

TEST(HealthcheckTest, FluentChainWithNullopts) {
  auto hc = Healthcheck::cmd_shell("test")
                .with_interval(std::nullopt)
                .with_timeout(std::nullopt)
                .with_retries(std::nullopt);
  EXPECT_TRUE(hc.is_valid());
}

// ====================
// Healthcheck Move Semantics Tests
// ====================

TEST(HealthcheckTest, MoveConstructor) {
  auto hc1 = Healthcheck::cmd_shell("test");
  auto hc2 = std::move(hc1);
  EXPECT_TRUE(hc2.is_valid());
}

TEST(HealthcheckTest, MoveAssignment) {
  auto hc1 = Healthcheck::cmd_shell("test1");
  auto hc2 = Healthcheck::cmd_shell("test2");
  hc2 = std::move(hc1);
  EXPECT_TRUE(hc2.is_valid());
}

// ====================
// Healthcheck Edge Cases Tests
// ====================

TEST(HealthcheckTest, VeryLongCommand) {
  std::string long_cmd(1000, 'a');
  auto hc = Healthcheck::cmd_shell(long_cmd);
  EXPECT_TRUE(hc.is_valid());
}

TEST(HealthcheckTest, CommandWithUnicode) {
  auto hc = Healthcheck::cmd_shell("echo 'Привет мир! 你好世界! 🚀'");
  EXPECT_TRUE(hc.is_valid());
}

TEST(HealthcheckTest, CommandWithSpecialChars) {
  auto hc = Healthcheck::cmd_shell("test -f /app/config && echo 'OK' || exit 1");
  EXPECT_TRUE(hc.is_valid());
}

TEST(HealthcheckTest, VeryLongInterval) {
  auto hc = Healthcheck::cmd_shell("test").with_interval(std::chrono::hours(24));
  EXPECT_TRUE(hc.is_valid());
}

TEST(HealthcheckTest, VeryShortInterval) {
  auto hc = Healthcheck::cmd_shell("test").with_interval(std::chrono::milliseconds(100));
  EXPECT_TRUE(hc.is_valid());
}

TEST(HealthcheckTest, HighRetryCount) {
  auto hc = Healthcheck::cmd_shell("test").with_retries(100);
  EXPECT_TRUE(hc.is_valid());
}

TEST(HealthcheckTest, CmdWithManyArgs) {
  std::vector<std::string> many_args;
  for (int i = 0; i < 50; ++i) {
    many_args.push_back("arg" + std::to_string(i));
  }
  auto hc = Healthcheck::cmd(many_args);
  EXPECT_TRUE(hc.is_valid());
}

