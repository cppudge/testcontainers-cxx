#include <rust/cxx.h>
#include <rust_tc_bridge/lib.h>

#include "testcontainers/core/wait/WaitFor.hpp"

#include "details/BoxHelper.hpp"

namespace testcontainers {

WaitFor::WaitFor(RsWaitFor *wait_for) noexcept
    : rimpl_(wait_for, [](RsWaitFor *w) { ::rs_wait_for_destroy(details::box_from_raw(w)); }) {}

WaitFor::WaitFor(const WaitFor &other) : WaitFor(other.rimpl_->rs_wait_for_clone().into_raw()) {}

WaitFor::WaitFor(WaitFor &&other) noexcept = default;

WaitFor &WaitFor::operator=(WaitFor &&other) noexcept = default;

WaitFor::~WaitFor() noexcept = default;

WaitFor WaitFor::Nothing() noexcept { return WaitFor(::rs_wait_for_nothing().into_raw()); }

WaitFor WaitFor::Log(LogWaitStrategy strategy) noexcept {
  return WaitFor(::rs_wait_for_log(details::into_box(strategy.rimpl_)).into_raw());
}

WaitFor WaitFor::Duration(std::chrono::duration<std::uint64_t, std::nano> duration) noexcept {
  return WaitFor(::rs_wait_for_duration(duration.count()).into_raw());
}

WaitFor WaitFor::Healthcheck(HealthWaitStrategy strategy) noexcept {
  return WaitFor(::rs_wait_for_healthcheck(details::into_box(strategy.rimpl_)).into_raw());
}

WaitFor WaitFor::Exit(ExitWaitStrategy strategy) noexcept {
  return WaitFor(::rs_wait_for_exit(details::into_box(strategy.rimpl_)).into_raw());
}

WaitFor WaitFor::message_on_stdout(std::string_view message) {
  return Log(LogWaitStrategy::std_out(message));
}

WaitFor WaitFor::message_on_stderr(std::string_view message) {
  return Log(LogWaitStrategy::std_err(message));
}

WaitFor WaitFor::message_on_either_std(std::string_view message) {
  return Log(LogWaitStrategy::stdout_or_stderr(message));
}

WaitFor WaitFor::log(LogWaitStrategy strategy) noexcept {
  return WaitFor::Log(std::move(strategy));
}

WaitFor WaitFor::healthcheck() noexcept {
  return WaitFor::Healthcheck(HealthWaitStrategy::healthcheck());
}

WaitFor WaitFor::exit() noexcept { return WaitFor::Exit(ExitWaitStrategy::exit()); }

WaitFor WaitFor::seconds(std::uint64_t seconds) noexcept {
  return WaitFor::Duration(std::chrono::seconds(seconds));
}

WaitFor WaitFor::millis(std::uint64_t milliseconds) noexcept {
  return WaitFor::Duration(std::chrono::milliseconds(milliseconds));
}

WaitFor WaitFor::millis_in_env_var(std::string_view name) {
  return WaitFor(::rs_wait_for_millis_in_env_var(details::into_str(name)).into_raw());
}

bool WaitFor::is_valid() const noexcept { return static_cast<bool>(rimpl_); }

} // namespace testcontainers
