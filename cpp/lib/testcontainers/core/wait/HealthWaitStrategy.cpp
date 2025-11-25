#include <rust/cxx.h>
#include <rust_tc_bridge/lib.h>

#include "testcontainers/core/wait/HealthWaitStrategy.hpp"

#include "details/BoxHelper.hpp"

namespace testcontainers {

HealthWaitStrategy::HealthWaitStrategy(RsHealthWaitStrategy *strategy) noexcept
    : rimpl_(strategy, [](RsHealthWaitStrategy *s) {
        ::rs_health_wait_strategy_destroy(details::box_from_raw(s));
      }) {}

HealthWaitStrategy::HealthWaitStrategy(HealthWaitStrategy &&other) noexcept = default;

HealthWaitStrategy &HealthWaitStrategy::operator=(HealthWaitStrategy &&other) noexcept = default;

HealthWaitStrategy::~HealthWaitStrategy() noexcept = default;

HealthWaitStrategy HealthWaitStrategy::healthcheck() noexcept {
  return HealthWaitStrategy(::rs_health_wait_strategy_new().into_raw());
}

HealthWaitStrategy HealthWaitStrategy::with_poll_interval(
    std::chrono::duration<std::uint64_t, std::nano> interval) noexcept {
  return HealthWaitStrategy(
      ::rs_health_wait_strategy_with_poll_interval(details::into_box(rimpl_), interval.count())
          .into_raw());
}

bool HealthWaitStrategy::is_valid() const noexcept { return static_cast<bool>(rimpl_); }

} // namespace testcontainers
