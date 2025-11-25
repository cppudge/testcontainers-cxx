#include <gtest/gtest.h>
#include <testcontainers/core/wait/WaitFor.hpp>

using namespace testcontainers;

TEST(WaitForTest, CreateNothing) {
  auto wait = WaitFor::Nothing();
  EXPECT_TRUE(wait.is_valid());
}

TEST(WaitForTest, CreateSeconds) {
  auto wait = WaitFor::seconds(5);
  EXPECT_TRUE(wait.is_valid());
}

TEST(WaitForTest, CreateMillis) {
  auto wait = WaitFor::millis(500);
  EXPECT_TRUE(wait.is_valid());
}

TEST(WaitForTest, CreateHealthcheck) {
  auto wait = WaitFor::healthcheck();
  EXPECT_TRUE(wait.is_valid());
}

TEST(WaitForTest, CreateExit) {
  auto wait = WaitFor::exit();
  EXPECT_TRUE(wait.is_valid());
}

TEST(WaitForTest, LogStdout) {
  auto wait = WaitFor::message_on_stdout("Ready");
  EXPECT_TRUE(wait.is_valid());
}

TEST(WaitForTest, LogStderr) {
  auto wait = WaitFor::message_on_stderr("Error");
  EXPECT_TRUE(wait.is_valid());
}

TEST(WaitForTest, LogEitherStd) {
  auto wait = WaitFor::message_on_either_std("Starting");
  EXPECT_TRUE(wait.is_valid());
}

TEST(WaitForTest, LogWaitStrategyWithTimes) {
  auto wait = WaitFor::Log(LogWaitStrategy::std_out("Message").with_times(3));
  EXPECT_TRUE(wait.is_valid());
}

TEST(WaitForTest, HealthWaitStrategyWithPollInterval) {
  auto wait = WaitFor::Healthcheck(
      HealthWaitStrategy::healthcheck().with_poll_interval(std::chrono::seconds(1)));
  EXPECT_TRUE(wait.is_valid());
}
