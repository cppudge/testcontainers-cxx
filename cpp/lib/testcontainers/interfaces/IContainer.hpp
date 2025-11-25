#pragma once

#include <cstdint>
#include <optional>
#include <vector>

namespace testcontainers {

class ContainerPort;
class ExecCommand;
class SyncExecResult;
class UrlHost;

class IContainer {
public:
  virtual ~IContainer() = default;

  virtual std::uint16_t get_host_port_ipv4(ContainerPort port) const = 0;
  virtual std::uint16_t get_host_port_ipv6(ContainerPort port) const = 0;
  virtual UrlHost get_host() const = 0;
  virtual SyncExecResult exec(ExecCommand cmd) const = 0;
  virtual void stop() const = 0;
  virtual void stop_with_timeout(std::optional<std::int32_t> timeout_sec) const = 0;
  virtual void start() const = 0;
  virtual std::vector<std::uint8_t> stdout_to_vec() const = 0;
  virtual std::vector<std::uint8_t> stderr_to_vec() const = 0;
  virtual bool is_running() const = 0;
  virtual std::optional<std::int64_t> exit_code() const = 0;
};

} // namespace testcontainers
