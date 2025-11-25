#pragma once

#include <chrono>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "testcontainers/interfaces/IImageExt.hpp"
#include "testcontainers/interfaces/IRustObject.hpp"
#include "testcontainers/interfaces/ISyncRunner.hpp"

class RsContainerRequest;

namespace testcontainers {

class Container;

class ContainerRequest final : public IRustObject, public IImageExt, public ISyncRunner {
public: // Default construction methods
  ContainerRequest(ContainerRequest &&other) noexcept;
  ContainerRequest &operator=(ContainerRequest &&other) noexcept;
  ~ContainerRequest() noexcept;
  ContainerRequest(const ContainerRequest &) = delete;
  ContainerRequest &operator=(const ContainerRequest &) = delete;

public: // IRustObject interface
  bool is_valid() const noexcept override;

public: // IImageExt interface
  ContainerRequest with_cmd(const std::vector<std::string> &cmd) override;
  ContainerRequest with_name(std::string_view name) override;
  ContainerRequest with_tag(std::string_view tag) override;
  ContainerRequest with_container_name(std::string_view name) override;
  ContainerRequest with_platform(std::string_view platform) override;
  ContainerRequest with_network(std::string_view network) override;
  ContainerRequest with_label(std::string_view key, std::string_view value) override;
  ContainerRequest
  with_labels(const std::vector<std::pair<std::string, std::string>> &labels) noexcept override;
  ContainerRequest with_host(std::string_view key, Host host) override;
  ContainerRequest with_mount(Mount mount) noexcept override;
  ContainerRequest with_env_var(std::string_view name, std::string_view value) override;
  ContainerRequest with_hostname(std::string_view hostname) override;
  ContainerRequest with_mapped_port(std::uint16_t host_port,
                                    ContainerPort container_port) noexcept override;
  ContainerRequest with_privileged(bool privileged) noexcept override;
  ContainerRequest with_cap_add(std::string_view capability) override;
  ContainerRequest with_cap_drop(std::string_view capability) override;
  ContainerRequest with_cgroupns_mode(CgroupnsMode mode) noexcept override;
  ContainerRequest with_userns_mode(std::string_view userns_mode) override;
  ContainerRequest with_shm_size(std::uint64_t bytes) noexcept override;
  ContainerRequest
  with_startup_timeout(std::chrono::duration<std::uint64_t, std::nano> timeout) noexcept override;
  ContainerRequest with_working_dir(std::string_view working_dir) override;
  ContainerRequest with_user(std::string_view user) override;
  ContainerRequest with_readonly_rootfs(bool readonly_rootfs) noexcept override;
  ContainerRequest with_security_opt(std::string_view security_opt) override;
  ContainerRequest with_ready_conditions(std::vector<WaitFor> ready_conditions) override;
  ContainerRequest with_copy_to(std::string_view target, CopyDataSource source) override;
  ContainerRequest with_ulimit(std::string_view name, std::int64_t soft,
                               std::optional<std::int64_t> hard) override;
  ContainerRequest with_health_check(Healthcheck health_check) noexcept override;

public: // ISyncRunner interface
  Container start() override;
  ContainerRequest pull() override;

private:
  friend class GenericImage;

  explicit ContainerRequest(RsContainerRequest *container_request) noexcept;

private:
  std::unique_ptr<RsContainerRequest, void (*)(RsContainerRequest *)> rimpl_;
};

} // namespace testcontainers
