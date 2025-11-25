#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "testcontainers/interfaces/IRustObject.hpp"

class RsSyncExecResult;

namespace testcontainers {

class SyncExecResult final : public IRustObject {
public:
  SyncExecResult(SyncExecResult &&other) noexcept;
  SyncExecResult &operator=(SyncExecResult &&other) noexcept;
  ~SyncExecResult() noexcept;
  SyncExecResult(const SyncExecResult &) = delete;
  SyncExecResult &operator=(const SyncExecResult &) = delete;

public:
  std::optional<std::int64_t> exit_code() noexcept;
  std::vector<std::uint8_t> stdout_to_vec() noexcept;
  std::vector<std::uint8_t> stderr_to_vec() noexcept;

  // TODO async readers

public:
  bool is_valid() const noexcept override;

public: // Helper methods
  std::string stdout_to_string();
  std::string stderr_to_string();

private:
  friend class Container;

  explicit SyncExecResult(RsSyncExecResult *result) noexcept;

private:
  std::unique_ptr<RsSyncExecResult, void (*)(RsSyncExecResult *)> rimpl_;
};

} // namespace testcontainers
