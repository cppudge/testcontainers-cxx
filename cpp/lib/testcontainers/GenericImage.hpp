#pragma once

#include <chrono>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "testcontainers/core/ContainerPort.hpp"
#include "testcontainers/core/wait/WaitFor.hpp"
#include "testcontainers/interfaces/IImage.hpp"
#include "testcontainers/interfaces/IImageExt.hpp"
#include "testcontainers/interfaces/IRustObject.hpp"
#include "testcontainers/interfaces/ISyncRunner.hpp"

class RsGenericImage;

namespace testcontainers {

class Container;
class ContainerRequest;

/**
 * @brief Builder for configuring and starting a container.
 *
 * Example:
 * @code
 * auto container = GenericImage("redis", "7.2.4")
 *     .with_exposed_port(6379)
 *     .with_wait_for_message("Ready to accept connections")
 *     .with_env("REDIS_PASSWORD", "secret")
 *     .start();
 * @endcode
 */
class GenericImage final : public IRustObject, public IImage, public IImageExt, public ISyncRunner {
public: // Static factory methods
  /**
   * @brief Construct a new Generic Image object
   *
   * @param name Image name (e.g., "redis")
   * @param tag Image tag (e.g., "7.2.4" or "latest")
   */
  explicit GenericImage(std::string_view name, std::string_view tag);

public: // Default construction methods
  GenericImage(GenericImage &&other) noexcept;
  GenericImage &operator=(GenericImage &&other) noexcept;
  ~GenericImage() noexcept;
  GenericImage(const GenericImage &) = delete;
  GenericImage &operator=(const GenericImage &) = delete;

public: // IRustObject interface
  bool is_valid() const noexcept override;

public: // Getters
  std::string name() const noexcept;
  std::string tag() const noexcept;
  std::optional<std::string> entrypoint() const noexcept;
  std::vector<ContainerPort> expose_ports() const noexcept;
  std::vector<WaitFor> ready_conditions() const noexcept;

public: // IImage interface
  GenericImage with_exposed_port(ContainerPort port) noexcept override;
  GenericImage with_entrypoint(std::string_view entrypoint) override;
  GenericImage with_wait_for(WaitFor wait_for) noexcept override;

public: // IImageExt interface
  ContainerRequest with_cmd(const std::vector<std::string> &cmd) override;
  ContainerRequest with_name(std::string_view name) override;
  ContainerRequest with_tag(std::string_view tag) override;
  ContainerRequest with_container_name(std::string_view name) override;
  ContainerRequest with_platform(std::string_view platform) override;
  ContainerRequest with_network(std::string_view network) override;
  ContainerRequest with_label(std::string_view key, std::string_view value) override;
  ContainerRequest with_labels(const std::vector<std::pair<std::string, std::string>> &labels) override;
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
  // WARNING: This will fail if hard is nullopt because of bug in testcontainers-rs???
  ContainerRequest with_ulimit(std::string_view name, std::int64_t soft,
                               std::optional<std::int64_t> hard) override;
  ContainerRequest with_health_check(Healthcheck health_check) noexcept override;

public: // ISyncRunner interface
  Container start() override;
  ContainerRequest pull() override;

private:
  friend class GenericBuildableImage;
  explicit GenericImage(RsGenericImage *image) noexcept;

private:
  std::unique_ptr<RsGenericImage, void (*)(RsGenericImage *)> rimpl_;
};

} // namespace testcontainers
