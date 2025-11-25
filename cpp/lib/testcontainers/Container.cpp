#include <rust/cxx.h>
#include <rust_tc_bridge/lib.h>

#include "testcontainers/Container.hpp"
#include "testcontainers/core/ExecCommand.hpp"
#include "testcontainers/core/SyncExecResult.hpp"
#include "testcontainers/system/UrlHost.hpp"
#include "details/OptionHelper.hpp"

#include "details/BoxHelper.hpp"
#include "details/ErrorHelper.hpp"

namespace testcontainers {

Container::Container(RsContainer *container) noexcept
    : rimpl_(container, [](RsContainer *c) { ::rs_container_destroy(details::box_from_raw(c)); }) {}

Container::Container(Container &&other) noexcept = default;

Container &Container::operator=(Container &&other) noexcept = default;

Container::~Container() noexcept = default;

bool Container::is_valid() const noexcept { return static_cast<bool>(rimpl_); }

std::uint16_t Container::get_host_port_ipv4(ContainerPort port) const {
  return details::call_map_error(&RsContainer::rs_container_get_host_port_ipv4, rimpl_.get(),
                                 details::into_box(port.rimpl_));
}

std::uint16_t Container::get_host_port_ipv6(ContainerPort port) const {
  return details::call_map_error(&RsContainer::rs_container_get_host_port_ipv6, rimpl_.get(),
                                 details::into_box(port.rimpl_));
}

UrlHost Container::get_host() const {
  return UrlHost(details::call_map_error(&RsContainer::rs_container_get_host, rimpl_.get()).into_raw());
}

SyncExecResult Container::exec(ExecCommand cmd) const {
  return SyncExecResult(
      details::call_map_error(&RsContainer::rs_container_exec, rimpl_.get(), details::into_box(cmd.rimpl_))
          .into_raw());
}

void Container::stop() const { details::call_map_error(&RsContainer::rs_container_stop, rimpl_.get()); }

void Container::stop_with_timeout(std::optional<std::int32_t> timeout_sec) const {
  details::call_map_error(&RsContainer::rs_container_stop_with_timeout, rimpl_.get(),
                           utils::optional_to_vec(std::move(timeout_sec)));
}

void Container::start() const { details::call_map_error(&RsContainer::rs_container_start, rimpl_.get()); }

void Container::rm(Container container) {
  details::call_map_error(::rs_container_rm, details::into_box(container.rimpl_));
}

std::vector<std::uint8_t> Container::stdout_to_vec() const noexcept {
  auto rust_vec = rimpl_->rs_container_stdout_to_vec();
  return std::vector<std::uint8_t>(rust_vec.begin(), rust_vec.end());
}

std::vector<std::uint8_t> Container::stderr_to_vec() const noexcept {
  auto rust_vec = rimpl_->rs_container_stderr_to_vec();
  return std::vector<std::uint8_t>(rust_vec.begin(), rust_vec.end());
}

bool Container::is_running() const noexcept { return rimpl_->rs_container_is_running(); }

std::optional<std::int64_t> Container::exit_code() const noexcept {
  return utils::vec_to_optional(rimpl_->rs_container_exit_code_opt());
}

std::string Container::stdout_to_string() const {
  const auto out = stdout_to_vec();
  return std::string(out.begin(), out.end());
}

std::string Container::stderr_to_string() const {
  const auto err = stderr_to_vec();
  return std::string(err.begin(), err.end());
}

} // namespace testcontainers
