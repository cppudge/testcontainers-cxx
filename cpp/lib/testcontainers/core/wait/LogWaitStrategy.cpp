#include <rust/cxx.h>
#include <rust_tc_bridge/lib.h>

#include "testcontainers/core/wait/LogWaitStrategy.hpp"

#include "details/BoxHelper.hpp"

namespace testcontainers {

LogWaitStrategy::LogWaitStrategy(RsLogWaitStrategy *strategy) noexcept
    : rimpl_(strategy, [](RsLogWaitStrategy *s) {
        ::rs_log_wait_strategy_destroy(details::box_from_raw(s));
      }) {}

LogWaitStrategy::LogWaitStrategy(LogWaitStrategy &&other) noexcept = default;

LogWaitStrategy &LogWaitStrategy::operator=(LogWaitStrategy &&other) noexcept = default;

LogWaitStrategy::~LogWaitStrategy() noexcept = default;

LogWaitStrategy LogWaitStrategy::std_out(std::string_view message) {
  return LogWaitStrategy(::rs_log_wait_strategy_stdout(details::into_string(message)).into_raw());
}

LogWaitStrategy LogWaitStrategy::std_err(std::string_view message) {
  return LogWaitStrategy(::rs_log_wait_strategy_stderr(details::into_string(message)).into_raw());
}

LogWaitStrategy LogWaitStrategy::stdout_or_stderr(std::string_view message) {
  return LogWaitStrategy(::rs_log_wait_strategy_stdout_or_stderr(details::into_string(message)).into_raw());
}

LogWaitStrategy LogWaitStrategy::with_times(std::size_t times) noexcept {
  return LogWaitStrategy(
      ::rs_log_wait_strategy_with_times(details::into_box(rimpl_), times).into_raw());
}

bool LogWaitStrategy::is_valid() const noexcept { return static_cast<bool>(rimpl_); }

} // namespace testcontainers
