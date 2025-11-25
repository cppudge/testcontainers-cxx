#pragma once

#include <memory>
#include <string_view>

#include "testcontainers/interfaces/IRustObject.hpp"

class RsHost;

namespace testcontainers {

class Host final : public IRustObject {
public: // Static factory methods
  static Host Addr(std::string_view ip_addr);
  static Host HostGatewayLinux() noexcept;

public: // Default construction methods
  Host(Host &&other) noexcept;
  Host &operator=(Host &&other) noexcept;
  ~Host() noexcept;
  Host(const Host &) = delete;
  Host &operator=(const Host &) = delete;

public: // IRustObject interface
  bool is_valid() const noexcept override;

private:
  friend class GenericImage;
  friend class ContainerRequest;

  explicit Host(RsHost *host) noexcept;

private:
  std::unique_ptr<RsHost, void (*)(RsHost *)> rimpl_;
};

} // namespace testcontainers
