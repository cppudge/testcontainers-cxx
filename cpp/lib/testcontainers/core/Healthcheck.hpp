#pragma once

#include <chrono>
#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "testcontainers/interfaces/IRustObject.hpp"

class RsHealthcheck;

namespace testcontainers {

class Healthcheck final : public IRustObject {
public: // Static factory methods
  static Healthcheck none() noexcept;
  static Healthcheck cmd_shell(std::string_view command) noexcept;
  static Healthcheck cmd(const std::vector<std::string> &command) noexcept;
  static Healthcheck empty() noexcept;

public: // Fluent methods
  Healthcheck with_interval(std::optional<std::chrono::duration<std::uint64_t, std::nano>> interval) noexcept;
  Healthcheck with_timeout(std::optional<std::chrono::duration<std::uint64_t, std::nano>> timeout) noexcept;
  Healthcheck with_retries(std::optional<std::uint32_t> retries) noexcept;
  Healthcheck with_start_period(std::optional<std::chrono::duration<std::uint64_t, std::nano>> start_period) noexcept;
  Healthcheck with_start_interval(std::optional<std::chrono::duration<std::uint64_t, std::nano>> interval) noexcept;

public: // Default construction methods
  Healthcheck(Healthcheck &&other) noexcept;
  Healthcheck &operator=(Healthcheck &&other) noexcept;
  ~Healthcheck() noexcept;
  Healthcheck(const Healthcheck &) = delete;
  Healthcheck &operator=(const Healthcheck &) = delete;

public: // IRustObject interface
  bool is_valid() const noexcept override;

private:
  friend class GenericImage;
  friend class ContainerRequest;

  explicit Healthcheck(RsHealthcheck *healthcheck) noexcept;

private:
  std::unique_ptr<RsHealthcheck, void (*)(RsHealthcheck *)> rimpl_;
};

} // namespace testcontainers
