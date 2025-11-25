#include <rust/cxx.h>
#include <rust_tc_bridge/lib.h>

#include "testcontainers/core/Healthcheck.hpp"

#include "details/BoxHelper.hpp"
#include "details/OptionHelper.hpp"
#include "details/VectorHelper.hpp"

namespace testcontainers {

Healthcheck::Healthcheck(RsHealthcheck *healthcheck) noexcept
    : rimpl_(healthcheck,
             [](RsHealthcheck *h) { ::rs_healthcheck_destroy(details::box_from_raw(h)); }) {}

Healthcheck::Healthcheck(Healthcheck &&other) noexcept = default;

Healthcheck &Healthcheck::operator=(Healthcheck &&other) noexcept = default;

Healthcheck::~Healthcheck() noexcept = default;

Healthcheck Healthcheck::none() noexcept { return Healthcheck(::rs_healthcheck_none().into_raw()); }

Healthcheck Healthcheck::cmd_shell(std::string_view command) noexcept {
  return Healthcheck(::rs_healthcheck_cmd_shell(details::into_string(command)).into_raw());
}

Healthcheck Healthcheck::cmd(const std::vector<std::string> &command) noexcept {
  return Healthcheck(::rs_healthcheck_cmd(utils::vector_to_vec<rust::String>(command)).into_raw());
}

Healthcheck Healthcheck::empty() noexcept { return Healthcheck(::rs_healthcheck_empty().into_raw()); }

Healthcheck Healthcheck::with_interval(
    std::optional<std::chrono::duration<std::uint64_t, std::nano>> interval) noexcept {
  const auto interval_ns_opt = utils::optional_to_vec<std::uint64_t>(
      std::move(interval), [](auto &&interval) { return interval.count(); });
  return Healthcheck(
      ::rs_healthcheck_with_interval(details::into_box(rimpl_), interval_ns_opt).into_raw());
}

Healthcheck Healthcheck::with_timeout(
    std::optional<std::chrono::duration<std::uint64_t, std::nano>> timeout) noexcept {
  const auto timeout_ns_opt = utils::optional_to_vec<std::uint64_t>(
      std::move(timeout), [](auto &&timeout) { return timeout.count(); });
  return Healthcheck(
      ::rs_healthcheck_with_timeout(details::into_box(rimpl_), timeout_ns_opt).into_raw());
}

Healthcheck Healthcheck::with_retries(std::optional<std::uint32_t> retries) noexcept {
  return Healthcheck(::rs_healthcheck_with_retries(details::into_box(rimpl_),
                                                    utils::optional_to_vec(std::move(retries)))
                         .into_raw());
}

Healthcheck Healthcheck::with_start_period(
    std::optional<std::chrono::duration<std::uint64_t, std::nano>> start_period) noexcept {
  const auto start_period_ns_opt = utils::optional_to_vec<std::uint64_t>(
      std::move(start_period), [](auto &&start_period) { return start_period.count(); });
  return Healthcheck(
      ::rs_healthcheck_with_start_period(details::into_box(rimpl_), start_period_ns_opt)
          .into_raw());
}

Healthcheck Healthcheck::with_start_interval(
    std::optional<std::chrono::duration<std::uint64_t, std::nano>> interval) noexcept {
  const auto interval_ns_opt = utils::optional_to_vec<std::uint64_t>(
      std::move(interval), [](auto &&interval) { return interval.count(); });
  return Healthcheck(
      ::rs_healthcheck_with_start_interval(details::into_box(rimpl_), interval_ns_opt)
          .into_raw());
}

bool Healthcheck::is_valid() const noexcept { return static_cast<bool>(rimpl_); }

} // namespace testcontainers
