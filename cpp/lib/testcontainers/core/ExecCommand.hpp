#pragma once

#include <memory>
#include <string>
#include <vector>

#include "testcontainers/interfaces/IRustObject.hpp"

class RsExecCommand;

namespace testcontainers {

class WaitFor;

class ExecCommand final : public IRustObject {
public:
  explicit ExecCommand(const std::vector<std::string> &cmd) noexcept;

  ExecCommand with_container_ready_conditions(std::vector<WaitFor> ready_conditions) noexcept;
  // TODO ExecCommand with_cmd_ready_condition(CmdWaitFor cmd_ready_condition) noexcept;

public:
  ExecCommand(ExecCommand &&other) noexcept;
  ExecCommand &operator=(ExecCommand &&other) noexcept;
  ~ExecCommand() noexcept;
  ExecCommand(const ExecCommand &) = delete;
  ExecCommand &operator=(const ExecCommand &) = delete;

public:
  bool is_valid() const noexcept override;

private:
  friend class Container;

  explicit ExecCommand(RsExecCommand *cmd) noexcept;

private:
  std::unique_ptr<RsExecCommand, void (*)(RsExecCommand *)> rimpl_;
};

} // namespace testcontainers
