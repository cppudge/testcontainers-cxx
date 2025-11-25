#pragma once

#include <cstdint>
#include <memory>

#include "testcontainers/interfaces/IRustObject.hpp"

class RsContainerPort;

namespace testcontainers {

class ContainerPort final : public IRustObject {
public: // Static factory methods
  static ContainerPort Tcp(std::uint16_t port) noexcept;
  static ContainerPort Udp(std::uint16_t port) noexcept;
  static ContainerPort Sctp(std::uint16_t port) noexcept;

public: // Default construction methods
  ContainerPort(ContainerPort &&other) noexcept;
  ContainerPort &operator=(ContainerPort &&other) noexcept;
  ~ContainerPort() noexcept;
  ContainerPort(const ContainerPort &) = delete;
  ContainerPort &operator=(const ContainerPort &) = delete;

public: // IRustObject interface
  bool is_valid() const noexcept override;

public: // Getters
  std::uint16_t as_u16() const noexcept;
  bool is_tcp() const noexcept;
  bool is_udp() const noexcept;
  bool is_sctp() const noexcept;

private:
  friend class GenericImage;
  friend class ContainerRequest;
  friend class Container;

  explicit ContainerPort(RsContainerPort *) noexcept;

private:
  std::unique_ptr<RsContainerPort, void (*)(RsContainerPort *)> rimpl_;
};

} // namespace testcontainers
