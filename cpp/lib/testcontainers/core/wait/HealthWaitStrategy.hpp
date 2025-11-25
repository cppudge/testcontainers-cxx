#pragma once

#include <chrono>
#include <memory>

#include "testcontainers/interfaces/IRustObject.hpp"

class RsHealthWaitStrategy;

namespace testcontainers {

class HealthWaitStrategy final : public IRustObject {
public: // Static factory methods
  static HealthWaitStrategy healthcheck() noexcept;

public: // Non-static factory methods that should be used in pair with static
        // factory methods
  HealthWaitStrategy with_poll_interval(std::chrono::duration<std::uint64_t, std::nano> interval) noexcept;

public: // Default construction methods
  HealthWaitStrategy(HealthWaitStrategy &&other) noexcept;
  HealthWaitStrategy &operator=(HealthWaitStrategy &&other) noexcept;
  ~HealthWaitStrategy() noexcept;
  HealthWaitStrategy(const HealthWaitStrategy &) = delete;
  HealthWaitStrategy &operator=(const HealthWaitStrategy &) = delete;

public: // IRustObject interface
  bool is_valid() const noexcept override;

private:
  friend class WaitFor;

  explicit HealthWaitStrategy(RsHealthWaitStrategy *strategy) noexcept;

private:
  std::unique_ptr<RsHealthWaitStrategy, void (*)(RsHealthWaitStrategy *)> rimpl_;
};

} // namespace testcontainers
