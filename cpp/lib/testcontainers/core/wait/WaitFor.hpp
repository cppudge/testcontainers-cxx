#pragma once

#include <chrono>
#include <memory>
#include <string_view>

#include "testcontainers/core/wait/ExitWaitStrategy.hpp"
#include "testcontainers/core/wait/HealthWaitStrategy.hpp"
#include "testcontainers/core/wait/LogWaitStrategy.hpp"
#include "testcontainers/interfaces/IRustObject.hpp"

class RsWaitFor;

namespace testcontainers {

class WaitFor final : public IRustObject {
public: // Static factory methods
  static WaitFor Nothing() noexcept;
  static WaitFor Log(LogWaitStrategy strategy) noexcept;
  static WaitFor Duration(std::chrono::duration<std::uint64_t, std::nano> duration) noexcept;
  static WaitFor Healthcheck(HealthWaitStrategy strategy) noexcept;
  static WaitFor Exit(ExitWaitStrategy strategy) noexcept;

public: // Static factory helpers
  static WaitFor message_on_stdout(std::string_view message);
  static WaitFor message_on_stderr(std::string_view message);
  static WaitFor message_on_either_std(std::string_view message);
  static WaitFor log(LogWaitStrategy strategy) noexcept;
  static WaitFor healthcheck() noexcept;
  static WaitFor exit() noexcept;
  static WaitFor seconds(std::uint64_t seconds) noexcept;
  static WaitFor millis(std::uint64_t milliseconds) noexcept;
  static WaitFor millis_in_env_var(std::string_view name);

public: // Default construction methods
  WaitFor(WaitFor &&other) noexcept;
  WaitFor &operator=(WaitFor &&other) noexcept;
  ~WaitFor() noexcept;
  WaitFor(const WaitFor &other);
  WaitFor &operator=(const WaitFor &other) = delete;

public: // IRustObject interface
  bool is_valid() const noexcept override;

private:
  friend class GenericImage;
  friend class ContainerRequest;
  friend class ExecCommand;

  explicit WaitFor(RsWaitFor *wait_for) noexcept;

private:
  std::unique_ptr<RsWaitFor, void (*)(RsWaitFor *)> rimpl_;
};

} // namespace testcontainers
