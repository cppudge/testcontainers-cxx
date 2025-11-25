#include <rust/cxx.h>
#include <rust_tc_bridge/lib.h>

#include "testcontainers/Container.hpp"
#include "testcontainers/ContainerRequest.hpp"
#include "testcontainers/core/CgroupnsMode.hpp"
#include "testcontainers/core/CopyDataSource.hpp"
#include "testcontainers/core/Healthcheck.hpp"
#include "testcontainers/core/Host.hpp"
#include "testcontainers/core/Mount.hpp"
#include "testcontainers/core/wait/WaitFor.hpp"
#include "details/OptionHelper.hpp"
#include "details/VectorHelper.hpp"


#include "details/BoxHelper.hpp"
#include "details/ErrorHelper.hpp"

namespace testcontainers {

ContainerRequest::ContainerRequest(RsContainerRequest *container_request) noexcept
    : rimpl_(container_request, [](RsContainerRequest *cr) {
        ::rs_container_request_destroy(details::box_from_raw(cr));
      }) {}

ContainerRequest::ContainerRequest(ContainerRequest &&other) noexcept = default;

ContainerRequest &ContainerRequest::operator=(ContainerRequest &&other) noexcept = default;

ContainerRequest::~ContainerRequest() noexcept = default;

bool ContainerRequest::is_valid() const noexcept { return static_cast<bool>(rimpl_); }

ContainerRequest ContainerRequest::with_cmd(const std::vector<std::string> &cmd) {
  return ContainerRequest(
      ::rs_container_request_with_cmd(details::into_box(rimpl_),
                                      utils::vector_to_vec<rust::String>(cmd))
          .into_raw());
}

ContainerRequest ContainerRequest::with_name(std::string_view name) {
  return ContainerRequest(
      ::rs_container_request_with_name(details::into_box(rimpl_), details::into_string(name))
          .into_raw());
}

ContainerRequest ContainerRequest::with_tag(std::string_view tag) {
  return ContainerRequest(
      ::rs_container_request_with_tag(details::into_box(rimpl_), details::into_string(tag))
          .into_raw());
}

ContainerRequest ContainerRequest::with_container_name(std::string_view name) {
  return ContainerRequest(::rs_container_request_with_container_name(details::into_box(rimpl_),
                                                                     details::into_string(name))
                              .into_raw());
}

ContainerRequest ContainerRequest::with_platform(std::string_view platform) {
  return ContainerRequest(::rs_container_request_with_platform(details::into_box(rimpl_),
                                                               details::into_string(platform))
                              .into_raw());
}

ContainerRequest ContainerRequest::with_network(std::string_view network) {
  return ContainerRequest(::rs_container_request_with_network(details::into_box(rimpl_),
                                                              details::into_string(network))
                              .into_raw());
}

ContainerRequest ContainerRequest::with_label(std::string_view key, std::string_view value) {
  return ContainerRequest(::rs_container_request_with_label(details::into_box(rimpl_),
                                                            details::into_string(key),
                                                            details::into_string(value))
                              .into_raw());
}

ContainerRequest
ContainerRequest::with_labels(const std::vector<std::pair<std::string, std::string>> &labels) noexcept {
  rust::Vec<rust::String> keys;
  rust::Vec<rust::String> values;
  keys.reserve(labels.size());
  values.reserve(labels.size());

  for (const auto &pair : labels) {
    keys.emplace_back(pair.first);
    values.emplace_back(pair.second);
  }

  return ContainerRequest(::rs_container_request_with_labels(details::into_box(rimpl_),
                                                             std::move(keys), std::move(values))
                              .into_raw());
}

ContainerRequest ContainerRequest::with_host(std::string_view key, Host host) {
  return ContainerRequest(::rs_container_request_with_host(details::into_box(rimpl_),
                                                           details::into_string(key),
                                                           details::into_box(host.rimpl_))
                              .into_raw());
}

ContainerRequest ContainerRequest::with_mount(Mount mount) noexcept {
  return ContainerRequest(::rs_container_request_with_mount(details::into_box(rimpl_),
                                                            details::into_box(mount.rimpl_))
                              .into_raw());
}

ContainerRequest ContainerRequest::with_env_var(std::string_view name, std::string_view value) {
  return ContainerRequest(::rs_container_request_with_env_var(details::into_box(rimpl_),
                                                              details::into_string(name),
                                                              details::into_string(value))
                              .into_raw());
}

ContainerRequest ContainerRequest::with_hostname(std::string_view hostname) {
  return ContainerRequest(::rs_container_request_with_hostname(details::into_box(rimpl_),
                                                               details::into_string(hostname))
                              .into_raw());
}

ContainerRequest ContainerRequest::with_mapped_port(std::uint16_t host_port,
                                                    ContainerPort container_port) noexcept {
  return ContainerRequest(
      ::rs_container_request_with_mapped_port(details::into_box(rimpl_), host_port,
                                              details::into_box(container_port.rimpl_))
          .into_raw());
}

ContainerRequest ContainerRequest::with_privileged(bool privileged) noexcept {
  return ContainerRequest(
      ::rs_container_request_with_privileged(details::into_box(rimpl_), privileged).into_raw());
}

ContainerRequest ContainerRequest::with_cap_add(std::string_view capability) {
  return ContainerRequest(::rs_container_request_with_cap_add(details::into_box(rimpl_),
                                                              details::into_string(capability))
                              .into_raw());
}

ContainerRequest ContainerRequest::with_cap_drop(std::string_view capability) {
  return ContainerRequest(::rs_container_request_with_cap_drop(details::into_box(rimpl_),
                                                               details::into_string(capability))
                              .into_raw());
}

ContainerRequest ContainerRequest::with_cgroupns_mode(CgroupnsMode mode) noexcept {
  return ContainerRequest(::rs_container_request_with_cgroupns_mode(
                              details::into_box(rimpl_), details::into_box(mode.rimpl_))
                              .into_raw());
}

ContainerRequest ContainerRequest::with_userns_mode(std::string_view userns_mode) {
  return ContainerRequest(::rs_container_request_with_userns_mode(details::into_box(rimpl_),
                                                                  details::into_str(userns_mode))
                              .into_raw());
}

ContainerRequest ContainerRequest::with_shm_size(std::uint64_t bytes) noexcept {
  return ContainerRequest(
      ::rs_container_request_with_shm_size(details::into_box(rimpl_), bytes).into_raw());
}

ContainerRequest ContainerRequest::with_startup_timeout(
    std::chrono::duration<std::uint64_t, std::nano> timeout) noexcept {
  return ContainerRequest(
      ::rs_container_request_with_startup_timeout(details::into_box(rimpl_), timeout.count())
          .into_raw());
}

ContainerRequest ContainerRequest::with_working_dir(std::string_view working_dir) {
  return ContainerRequest(::rs_container_request_with_working_dir(details::into_box(rimpl_),
                                                                  details::into_string(working_dir))
                              .into_raw());
}

ContainerRequest ContainerRequest::with_user(std::string_view user) {
  return ContainerRequest(
      ::rs_container_request_with_user(details::into_box(rimpl_), details::into_string(user))
          .into_raw());
}

ContainerRequest ContainerRequest::with_readonly_rootfs(bool readonly_rootfs) noexcept {
  return ContainerRequest(
      ::rs_container_request_with_readonly_rootfs(details::into_box(rimpl_), readonly_rootfs)
          .into_raw());
}

ContainerRequest ContainerRequest::with_security_opt(std::string_view security_opt) {
  return ContainerRequest(::rs_container_request_with_security_opt(details::into_box(rimpl_),
                                                                   details::into_string(security_opt))
                              .into_raw());
}

ContainerRequest ContainerRequest::with_ready_conditions(std::vector<WaitFor> ready_conditions) {
  rust::Vec<RsWaitFor> rust_conditions;
  rust_conditions.reserve(ready_conditions.size());
  for (auto &wf : ready_conditions) {
    ::rs_wait_for_vec_push(rust_conditions, details::into_box(wf.rimpl_));
  }
  return ContainerRequest(::rs_container_request_with_ready_conditions(
                              details::into_box(rimpl_), std::move(rust_conditions))
                              .into_raw());
}

ContainerRequest ContainerRequest::with_copy_to(std::string_view target, CopyDataSource source) {
  return ContainerRequest(::rs_container_request_with_copy_to(details::into_box(rimpl_),
                                                              details::into_string(target),
                                                              details::into_box(source.rimpl_))
                              .into_raw());
}

ContainerRequest ContainerRequest::with_ulimit(std::string_view name, std::int64_t soft,
                                               std::optional<std::int64_t> hard) {
  return ContainerRequest(
      ::rs_container_request_with_ulimit(details::into_box(rimpl_), details::into_str(name), soft,
                                         utils::optional_to_vec(std::move(hard)))
          .into_raw());
}

ContainerRequest ContainerRequest::with_health_check(Healthcheck health_check) noexcept {
  return ContainerRequest(
      ::rs_container_request_with_health_check(details::into_box(rimpl_),
                                               details::into_box(health_check.rimpl_))
          .into_raw());
}

Container ContainerRequest::start() {
  return Container(details::call_map_error([&] {
                     return ::rs_container_request_start(details::into_box(rimpl_));
                   }).into_raw());
}

ContainerRequest ContainerRequest::pull() {
  return ContainerRequest(details::call_map_error([&] {
                            return ::rs_container_request_pull(details::into_box(rimpl_));
                          }).into_raw());
}

} // namespace testcontainers
