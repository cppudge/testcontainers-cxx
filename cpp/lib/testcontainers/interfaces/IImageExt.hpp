#pragma once

#include <chrono>
#include <cstdint>
#include <optional>
#include <string>
#include <utility>
#include <vector>

namespace testcontainers {

class ContainerPort;
class ContainerRequest;
class CopyDataSource;
class Healthcheck;
class Host;
class Mount;
class WaitFor;
class CgroupnsMode;

class IImageExt {
public:
  virtual ~IImageExt() = default;

  virtual ContainerRequest with_cmd(const std::vector<std::string> &cmd) = 0;
  virtual ContainerRequest with_name(std::string_view name) = 0;
  virtual ContainerRequest with_tag(std::string_view tag) = 0;
  virtual ContainerRequest with_container_name(std::string_view name) = 0;
  virtual ContainerRequest with_platform(std::string_view platform) = 0;
  virtual ContainerRequest with_network(std::string_view network) = 0;
  virtual ContainerRequest with_label(std::string_view key, std::string_view value) = 0;
  virtual ContainerRequest with_labels(const std::vector<std::pair<std::string, std::string>> &labels) = 0;
  virtual ContainerRequest with_env_var(std::string_view name, std::string_view value) = 0;
  virtual ContainerRequest with_host(std::string_view key, Host host) = 0;
  virtual ContainerRequest with_hostname(std::string_view hostname) = 0;
  virtual ContainerRequest with_mount(Mount mount) noexcept = 0;
  virtual ContainerRequest with_copy_to(std::string_view target, CopyDataSource source) = 0;
  virtual ContainerRequest with_mapped_port(std::uint16_t host_port,
                                            ContainerPort container_port) noexcept
      = 0;
  // with_exposed_host_port (#[cfg(feature = "host-port-exposure")])
  // with_exposed_host_ports (#[cfg(feature = "host-port-exposure")])
  virtual ContainerRequest with_ulimit(std::string_view name, std::int64_t soft,
                                       std::optional<std::int64_t> hard)
      = 0;
  virtual ContainerRequest with_privileged(bool privileged) noexcept = 0;
  virtual ContainerRequest with_cap_add(std::string_view capability) = 0;
  virtual ContainerRequest with_cap_drop(std::string_view capability) = 0;
  virtual ContainerRequest with_cgroupns_mode(CgroupnsMode mode) noexcept = 0;
  virtual ContainerRequest with_userns_mode(std::string_view userns_mode) = 0;
  virtual ContainerRequest with_shm_size(std::uint64_t bytes) noexcept = 0;
  virtual ContainerRequest
  with_startup_timeout(std::chrono::duration<std::uint64_t, std::nano> timeout) noexcept
      = 0;
  virtual ContainerRequest with_working_dir(std::string_view working_dir) = 0;
  // with_log_consumer (TODO.md)
  // with_reuse (#[cfg(feature = "reusable-containers")])
  virtual ContainerRequest with_user(std::string_view user) = 0;
  virtual ContainerRequest with_readonly_rootfs(bool readonly_rootfs) noexcept = 0;
  virtual ContainerRequest with_security_opt(std::string_view security_opt) = 0;
  virtual ContainerRequest with_ready_conditions(std::vector<WaitFor> ready_conditions) = 0;
  virtual ContainerRequest with_health_check(Healthcheck health_check) noexcept = 0;
  // with_device_requests (#[cfg(feature = "device-requests")])
};

} // namespace testcontainers
