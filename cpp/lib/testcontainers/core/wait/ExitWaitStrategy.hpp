#pragma once

#include <chrono>
#include <memory>

#include "testcontainers/interfaces/IRustObject.hpp"

class RsExitWaitStrategy;

namespace testcontainers {

class ExitWaitStrategy final : public IRustObject {
public: // Static factory methods
  static ExitWaitStrategy exit() noexcept;

public: // Non-static factory methods that should be used in pair with static
        // factory methods
  ExitWaitStrategy with_poll_interval(std::chrono::duration<std::uint64_t, std::nano> interval) noexcept;

  ExitWaitStrategy with_exit_code(std::int64_t exit_code) noexcept;

public: // Default construction methods
  ExitWaitStrategy(ExitWaitStrategy &&other) noexcept;
  ExitWaitStrategy &operator=(ExitWaitStrategy &&other) noexcept;
  ~ExitWaitStrategy() noexcept;
  ExitWaitStrategy(const ExitWaitStrategy &) = delete;
  ExitWaitStrategy &operator=(const ExitWaitStrategy &) = delete;

public: // IRustObject interface
  bool is_valid() const noexcept override;

private:
  friend class WaitFor;

  explicit ExitWaitStrategy(RsExitWaitStrategy *strategy) noexcept;

private:
  std::unique_ptr<RsExitWaitStrategy, void (*)(RsExitWaitStrategy *)> rimpl_;
};

} // namespace testcontainers
