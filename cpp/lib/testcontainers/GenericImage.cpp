#include <rust/cxx.h>
#include <rust_tc_bridge/lib.h>

#include "testcontainers/Container.hpp"
#include "testcontainers/ContainerRequest.hpp"
#include "testcontainers/GenericImage.hpp"
#include "testcontainers/core/CgroupnsMode.hpp"
#include "testcontainers/core/CopyDataSource.hpp"
#include "testcontainers/core/Healthcheck.hpp"
#include "testcontainers/core/Host.hpp"
#include "testcontainers/core/Mount.hpp"
#include "details/OptionHelper.hpp"
#include "details/VectorHelper.hpp"

#include "details/BoxHelper.hpp"
#include "details/ErrorHelper.hpp"

namespace testcontainers {

GenericImage::GenericImage(RsGenericImage *image) noexcept
    : rimpl_(image, [](RsGenericImage *image) {
        ::rs_generic_image_destroy(rust::Box<RsGenericImage>::from_raw(image));
      }) {}

GenericImage::GenericImage(std::string_view name, std::string_view tag)
    : GenericImage(::rs_generic_image_new(details::into_string(name), details::into_string(tag)).into_raw()) {}

GenericImage::GenericImage(GenericImage &&other) noexcept = default;

GenericImage &GenericImage::operator=(GenericImage &&other) noexcept = default;

GenericImage::~GenericImage() noexcept = default;

GenericImage GenericImage::with_exposed_port(ContainerPort port) noexcept {
  return GenericImage(::rs_generic_image_with_exposed_port(details::into_box(rimpl_),
                                                           details::into_box(port.rimpl_))
                          .into_raw());
}

GenericImage GenericImage::with_entrypoint(std::string_view entrypoint) {
  return GenericImage(
      ::rs_generic_image_with_entrypoint(details::into_box(rimpl_),
                                         details::into_str(entrypoint))
          .into_raw());
}

GenericImage GenericImage::with_wait_for(WaitFor wait_for) noexcept {
  return GenericImage(::rs_generic_image_with_wait_for(details::into_box(rimpl_),
                                                       details::into_box(wait_for.rimpl_))
                          .into_raw());
}

bool GenericImage::is_valid() const noexcept { return static_cast<bool>(rimpl_); }

std::string GenericImage::name() const noexcept {
  return std::string(rimpl_->rs_generic_image_name());
}

std::string GenericImage::tag() const noexcept {
  return std::string(rimpl_->rs_generic_image_tag());
}

std::optional<std::string> GenericImage::entrypoint() const noexcept {
  auto vec = rimpl_->rs_generic_image_entrypoint_opt();
  if (vec.empty()) {
    return std::nullopt;
  }
  return std::string(vec[0]);
}

std::vector<ContainerPort> GenericImage::expose_ports() const noexcept {
  auto rust_vec = rimpl_->rs_generic_image_expose_ports();
  std::vector<ContainerPort> result;

  while (!rust_vec.empty()) {
    result.insert(result.begin(), ContainerPort(::rs_container_port_vec_pop(rust_vec).into_raw()));
  }

  return result;
}

std::vector<WaitFor> GenericImage::ready_conditions() const noexcept {
  auto rust_vec = rimpl_->rs_generic_image_ready_conditions();
  std::vector<WaitFor> result;

  while (!rust_vec.empty()) {
    result.insert(result.begin(), WaitFor(::rs_wait_for_vec_pop(rust_vec).into_raw()));
  }

  return result;
}

ContainerRequest GenericImage::with_cmd(const std::vector<std::string> &cmd) {
  return ContainerRequest(
      ::rs_generic_image_with_cmd(details::into_box(rimpl_),
                                  utils::vector_to_vec<rust::String>(cmd))
          .into_raw());
}

ContainerRequest GenericImage::with_name(std::string_view name) {
  return ContainerRequest(
      ::rs_generic_image_with_name(details::into_box(rimpl_), details::into_string(name))
          .into_raw());
}

ContainerRequest GenericImage::with_tag(std::string_view tag) {
  return ContainerRequest(
      ::rs_generic_image_with_tag(details::into_box(rimpl_), details::into_string(tag))
          .into_raw());
}

ContainerRequest GenericImage::with_container_name(std::string_view name) {
  return ContainerRequest(::rs_generic_image_with_container_name(details::into_box(rimpl_),
                                                                 details::into_string(name))
                              .into_raw());
}

ContainerRequest GenericImage::with_platform(std::string_view platform) {
  return ContainerRequest(
      ::rs_generic_image_with_platform(details::into_box(rimpl_), details::into_string(platform))
          .into_raw());
}

ContainerRequest GenericImage::with_network(std::string_view network) {
  return ContainerRequest(
      ::rs_generic_image_with_network(details::into_box(rimpl_), details::into_string(network))
          .into_raw());
}

ContainerRequest GenericImage::with_label(std::string_view key, std::string_view value) {
  return ContainerRequest(::rs_generic_image_with_label(details::into_box(rimpl_),
                                                        details::into_string(key),
                                                        details::into_string(value))
                              .into_raw());
}

ContainerRequest
GenericImage::with_labels(const std::vector<std::pair<std::string, std::string>> &labels) {
  rust::Vec<rust::String> keys;
  rust::Vec<rust::String> values;
  keys.reserve(labels.size());
  values.reserve(labels.size());
  for (const auto &pair : labels) {
    keys.emplace_back(pair.first);
    values.emplace_back(pair.second);
  }
  return ContainerRequest(::rs_generic_image_with_labels(details::into_box(rimpl_),
                                                         std::move(keys), std::move(values))
                              .into_raw());
}

ContainerRequest GenericImage::with_host(std::string_view key, Host host) {
  return ContainerRequest(::rs_generic_image_with_host(details::into_box(rimpl_),
                                                       details::into_string(key),
                                                       details::into_box(host.rimpl_))
                              .into_raw());
}

ContainerRequest GenericImage::with_mount(Mount mount) noexcept {
  return ContainerRequest(::rs_generic_image_with_mount(details::into_box(rimpl_),
                                                        details::into_box(mount.rimpl_))
                              .into_raw());
}

ContainerRequest GenericImage::with_env_var(std::string_view name, std::string_view value) {
  return ContainerRequest(::rs_generic_image_with_env_var(details::into_box(rimpl_),
                                                          details::into_string(name),
                                                          details::into_string(value))
                              .into_raw());
}

ContainerRequest GenericImage::with_hostname(std::string_view hostname) {
  return ContainerRequest(
      ::rs_generic_image_with_hostname(details::into_box(rimpl_), details::into_string(hostname))
          .into_raw());
}

ContainerRequest GenericImage::with_mapped_port(std::uint16_t host_port,
                                                ContainerPort container_port) noexcept {
  return ContainerRequest(
      ::rs_generic_image_with_mapped_port(details::into_box(rimpl_), host_port,
                                          details::into_box(container_port.rimpl_))
          .into_raw());
}

ContainerRequest GenericImage::with_privileged(bool privileged) noexcept {
  return ContainerRequest(
      ::rs_generic_image_with_privileged(details::into_box(rimpl_), privileged).into_raw());
}

ContainerRequest GenericImage::with_cap_add(std::string_view capability) {
  return ContainerRequest(
      ::rs_generic_image_with_cap_add(details::into_box(rimpl_), details::into_string(capability))
          .into_raw());
}

ContainerRequest GenericImage::with_cap_drop(std::string_view capability) {
  return ContainerRequest(::rs_generic_image_with_cap_drop(details::into_box(rimpl_),
                                                           details::into_string(capability))
                              .into_raw());
}

ContainerRequest GenericImage::with_cgroupns_mode(CgroupnsMode mode) noexcept {
  return ContainerRequest(::rs_generic_image_with_cgroupns_mode(details::into_box(rimpl_),
                                                                details::into_box(mode.rimpl_))
                              .into_raw());
}

ContainerRequest GenericImage::with_userns_mode(std::string_view userns_mode) {
  return ContainerRequest(::rs_generic_image_with_userns_mode(details::into_box(rimpl_),
                                                              details::into_str(userns_mode))
                              .into_raw());
}

ContainerRequest GenericImage::with_shm_size(std::uint64_t bytes) noexcept {
  return ContainerRequest(
      ::rs_generic_image_with_shm_size(details::into_box(rimpl_), bytes).into_raw());
}

ContainerRequest GenericImage::with_startup_timeout(
    std::chrono::duration<std::uint64_t, std::nano> timeout) noexcept {
  return ContainerRequest(
      ::rs_generic_image_with_startup_timeout(details::into_box(rimpl_), timeout.count())
          .into_raw());
}

ContainerRequest GenericImage::with_working_dir(std::string_view working_dir) {
  return ContainerRequest(::rs_generic_image_with_working_dir(details::into_box(rimpl_),
                                                              details::into_string(working_dir))
                              .into_raw());
}

ContainerRequest GenericImage::with_user(std::string_view user) {
  return ContainerRequest(
      ::rs_generic_image_with_user(details::into_box(rimpl_), details::into_string(user))
          .into_raw());
}

ContainerRequest GenericImage::with_readonly_rootfs(bool readonly_rootfs) noexcept {
  return ContainerRequest(
      ::rs_generic_image_with_readonly_rootfs(details::into_box(rimpl_), readonly_rootfs)
          .into_raw());
}

ContainerRequest GenericImage::with_security_opt(std::string_view security_opt) {
  return ContainerRequest(::rs_generic_image_with_security_opt(details::into_box(rimpl_),
                                                               details::into_string(security_opt))
                              .into_raw());
}

ContainerRequest GenericImage::with_ready_conditions(std::vector<WaitFor> ready_conditions) {
  rust::Vec<RsWaitFor> rust_conditions;
  rust_conditions.reserve(ready_conditions.size());
  for (auto &wf : ready_conditions) {
    ::rs_wait_for_vec_push(rust_conditions, details::into_box(wf.rimpl_));
  }
  return ContainerRequest(::rs_generic_image_with_ready_conditions(details::into_box(rimpl_),
                                                                   std::move(rust_conditions))
                              .into_raw());
}

ContainerRequest GenericImage::with_copy_to(std::string_view target, CopyDataSource source) {
  return ContainerRequest(::rs_generic_image_with_copy_to(details::into_box(rimpl_),
                                                          details::into_string(target),
                                                          details::into_box(source.rimpl_))
                              .into_raw());
}

ContainerRequest GenericImage::with_ulimit(std::string_view name, std::int64_t soft,
                                           std::optional<std::int64_t> hard) {
  return ContainerRequest(::rs_generic_image_with_ulimit(details::into_box(rimpl_),
                                                         details::into_str(name), soft,
                                                         utils::optional_to_vec(std::move(hard)))
                              .into_raw());
}

ContainerRequest GenericImage::with_health_check(Healthcheck health_check) noexcept {
  return ContainerRequest(
      ::rs_generic_image_with_health_check(details::into_box(rimpl_),
                                           details::into_box(health_check.rimpl_))
          .into_raw());
}

Container GenericImage::start() {
  return Container(details::call_map_error([&] {
                     return ::rs_generic_image_start(details::into_box(rimpl_));
                   }).into_raw());
}

ContainerRequest GenericImage::pull() {
  return ContainerRequest(details::call_map_error([&] {
                            return ::rs_generic_image_pull(details::into_box(rimpl_));
                          }).into_raw());
}

} // namespace testcontainers
