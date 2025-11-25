#pragma once

#include <memory>
#include <string_view>

#include "testcontainers/interfaces/IRustObject.hpp"

class RsLogWaitStrategy;

namespace testcontainers {

/**
 * @brief Wait strategy that waits for a specific message in container logs.
 *
 * This class provides a fluent interface for configuring log-based wait
 * conditions.
 *
 * Example:
 * @code
 * auto strategy = LogWaitStrategy::stdout("Ready to accept connections")
 *                     .with_times(2);  // Wait for message to appear twice
 * @endcode
 */
class LogWaitStrategy final : public IRustObject {
public: // Static factory methods
  static LogWaitStrategy std_out(std::string_view message);
  static LogWaitStrategy std_err(std::string_view message);
  static LogWaitStrategy stdout_or_stderr(std::string_view message);

public: // Non-static factory methods that should be used in pair with static
        // factory methods
  LogWaitStrategy with_times(std::size_t times) noexcept;

public: // Default construction methods
  LogWaitStrategy(LogWaitStrategy &&other) noexcept;
  LogWaitStrategy &operator=(LogWaitStrategy &&other) noexcept;
  ~LogWaitStrategy() noexcept;
  LogWaitStrategy(const LogWaitStrategy &) = delete;
  LogWaitStrategy &operator=(const LogWaitStrategy &) = delete;

public: // IRustObject interface
  bool is_valid() const noexcept override;

private:
  friend class WaitFor;

  explicit LogWaitStrategy(RsLogWaitStrategy *strategy) noexcept;

private:
  std::unique_ptr<RsLogWaitStrategy, void (*)(RsLogWaitStrategy *)> rimpl_;
};

} // namespace testcontainers
