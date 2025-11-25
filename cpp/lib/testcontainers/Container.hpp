#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "testcontainers/core/ContainerPort.hpp"
#include "testcontainers/interfaces/IContainer.hpp"
#include "testcontainers/interfaces/IRustObject.hpp"

class RsContainer;

namespace testcontainers {

/**
 * @brief RAII wrapper for a running container.
 *
 * Container is automatically stopped when destroyed unless explicitly stopped.
 *
 * Example:
 * @code
 * {
 *     auto container = GenericImage("redis", "7.2.4")
 *         .with_exposed_port(6379)
 *         .start();
 *
 *     auto host_port = container.mapped_port(6379);
 *     // Use container...
 * } // Container automatically stopped here
 * @endcode
 */
class Container final : public IRustObject, public IContainer {
public: // Default construction methods
  Container(Container &&other) noexcept;
  Container &operator=(Container &&other) noexcept;
  ~Container() noexcept;
  Container(const Container &) = delete;
  Container &operator=(const Container &) = delete;

public: // IRustObject interface
  bool is_valid() const noexcept override;

public: // IContainer interface
  std::uint16_t get_host_port_ipv4(ContainerPort port) const override;
  std::uint16_t get_host_port_ipv6(ContainerPort port) const override;
  UrlHost get_host() const override;
  SyncExecResult exec(ExecCommand cmd) const override;
  void stop() const override;
  void stop_with_timeout(std::optional<std::int32_t> timeout_sec) const override;
  void start() const override;
  std::vector<std::uint8_t> stdout_to_vec() const noexcept override;
  std::vector<std::uint8_t> stderr_to_vec() const noexcept override;
  bool is_running() const noexcept override;
  std::optional<std::int64_t> exit_code() const noexcept override;

  static void rm(Container container);

public: // Helper methods
  std::string stdout_to_string() const;
  std::string stderr_to_string() const;

private:
  friend class GenericImage;
  friend class ContainerRequest;

  explicit Container(RsContainer *container) noexcept;

private:
  std::unique_ptr<RsContainer, void (*)(RsContainer *)> rimpl_;
};

} // namespace testcontainers
