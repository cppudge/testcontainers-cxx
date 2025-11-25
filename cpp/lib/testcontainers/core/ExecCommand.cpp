#include <rust/cxx.h>
#include <rust_tc_bridge/lib.h>

#include "testcontainers/core/ExecCommand.hpp"
#include "testcontainers/core/wait/WaitFor.hpp"

#include "details/BoxHelper.hpp"
#include "details/VectorHelper.hpp"

namespace testcontainers {

ExecCommand::ExecCommand(RsExecCommand *cmd) noexcept
    : rimpl_(cmd, [](RsExecCommand *c) { ::rs_exec_command_destroy(details::box_from_raw(c)); }) {}

ExecCommand::ExecCommand(const std::vector<std::string> &cmd) noexcept
    : ExecCommand(::rs_exec_command_new(utils::vector_to_vec<rust::String>(cmd)).into_raw()) {}

ExecCommand::ExecCommand(ExecCommand &&other) noexcept = default;

ExecCommand &ExecCommand::operator=(ExecCommand &&other) noexcept = default;

ExecCommand::~ExecCommand() noexcept = default;

ExecCommand ExecCommand::with_container_ready_conditions(std::vector<WaitFor> ready_conditions) noexcept {
  rust::Vec<RsWaitFor> rust_conditions;
  rust_conditions.reserve(ready_conditions.size());
  for (auto &wf : ready_conditions) {
    ::rs_wait_for_vec_push(rust_conditions, details::into_box(wf.rimpl_));
  }

  return ExecCommand(
      ::rs_exec_command_with_container_ready_conditions(details::into_box(rimpl_),
                                                        std::move(rust_conditions))
          .into_raw());
}

bool ExecCommand::is_valid() const noexcept { return static_cast<bool>(rimpl_); }

} // namespace testcontainers
