#include <rust/cxx.h>
#include <rust_tc_bridge/lib.h>

#include "testcontainers/core/SyncExecResult.hpp"

#include "details/BoxHelper.hpp"
#include "details/OptionHelper.hpp"
#include "details/VectorHelper.hpp"

namespace testcontainers {

SyncExecResult::SyncExecResult(RsSyncExecResult *result) noexcept
    : rimpl_(result, [](RsSyncExecResult *r) {
        ::rs_sync_exec_result_destroy(details::box_from_raw(r));
      }) {}

SyncExecResult::SyncExecResult(SyncExecResult &&other) noexcept = default;

SyncExecResult &SyncExecResult::operator=(SyncExecResult &&other) noexcept = default;

SyncExecResult::~SyncExecResult() noexcept = default;

std::optional<std::int64_t> SyncExecResult::exit_code() noexcept {
  return utils::vec_to_optional(::rs_sync_exec_result_exit_code_opt(*rimpl_));
}

std::vector<std::uint8_t> SyncExecResult::stdout_to_vec() noexcept {
  return utils::vec_to_vector(::rs_sync_exec_result_stdout_to_vec(*rimpl_));
}

std::vector<std::uint8_t> SyncExecResult::stderr_to_vec() noexcept {
  return utils::vec_to_vector(::rs_sync_exec_result_stderr_to_vec(*rimpl_));
}

std::string SyncExecResult::stdout_to_string() {
  const auto out = stdout_to_vec();
  return std::string(out.begin(), out.end());
}

std::string SyncExecResult::stderr_to_string() {
  const auto err = stderr_to_vec();
  return std::string(err.begin(), err.end());
}

bool SyncExecResult::is_valid() const noexcept { return static_cast<bool>(rimpl_); }

} // namespace testcontainers
