#include <rust/cxx.h>
#include <rust_tc_bridge/lib.h>

#include "testcontainers/core/wait/ExitWaitStrategy.hpp"

#include "details/BoxHelper.hpp"

namespace testcontainers {

ExitWaitStrategy::ExitWaitStrategy(RsExitWaitStrategy *strategy) noexcept
    : rimpl_(strategy, [](RsExitWaitStrategy *s) {
        ::rs_exit_wait_strategy_destroy(details::box_from_raw(s));
      }) {}

ExitWaitStrategy::ExitWaitStrategy(ExitWaitStrategy &&other) noexcept = default;

ExitWaitStrategy &ExitWaitStrategy::operator=(ExitWaitStrategy &&other) noexcept = default;

ExitWaitStrategy::~ExitWaitStrategy() noexcept = default;

ExitWaitStrategy ExitWaitStrategy::exit() noexcept {
  return ExitWaitStrategy(::rs_exit_wait_strategy_new().into_raw());
}

ExitWaitStrategy
ExitWaitStrategy::with_poll_interval(std::chrono::duration<std::uint64_t, std::nano> interval) noexcept {
  return ExitWaitStrategy(
      ::rs_exit_wait_strategy_with_poll_interval(details::into_box(rimpl_), interval.count())
          .into_raw());
}

ExitWaitStrategy ExitWaitStrategy::with_exit_code(std::int64_t exit_code) noexcept {
  return ExitWaitStrategy(
      ::rs_exit_wait_strategy_with_exit_code(details::into_box(rimpl_), exit_code).into_raw());
}

bool ExitWaitStrategy::is_valid() const noexcept { return static_cast<bool>(rimpl_); }

} // namespace testcontainers
