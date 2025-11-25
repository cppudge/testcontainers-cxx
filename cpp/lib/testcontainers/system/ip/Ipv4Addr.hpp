#pragma once

#include <array>
#include <cstdint>
#include <memory>
#include <string>

#include "testcontainers/interfaces/IRustObject.hpp"

class RsIpv4Addr;

namespace testcontainers {

class Ipv4Addr final : public IRustObject {
public:
  static constexpr std::uint32_t BITS = 32;

  explicit Ipv4Addr(std::uint8_t a, std::uint8_t b, std::uint8_t c, std::uint8_t d) noexcept;

  static Ipv4Addr from_bits(std::uint32_t bits) noexcept;
  static Ipv4Addr localhost() noexcept;
  static Ipv4Addr unspecified() noexcept;
  static Ipv4Addr broadcast() noexcept;
  static Ipv4Addr from_octets(const std::array<std::uint8_t, 4> &octets) noexcept;

  bool is_unspecified() const noexcept;
  bool is_loopback() const noexcept;
  bool is_private() const noexcept;
  bool is_link_local() const noexcept;
  bool is_multicast() const noexcept;
  bool is_broadcast() const noexcept;
  bool is_documentation() const noexcept;

  std::uint32_t to_bits() const noexcept;
  std::array<std::uint8_t, 4> octets() const noexcept;
  std::string to_string() const noexcept;

public:
  Ipv4Addr(Ipv4Addr &&other) noexcept;
  Ipv4Addr &operator=(Ipv4Addr &&other) noexcept;
  ~Ipv4Addr() noexcept;
  Ipv4Addr(const Ipv4Addr &) = delete;
  Ipv4Addr &operator=(const Ipv4Addr &) = delete;

public:
  bool is_valid() const noexcept override;

private:
  friend class IpAddr;
  friend class UrlHost;

  explicit Ipv4Addr(RsIpv4Addr *addr) noexcept;

private:
  std::unique_ptr<RsIpv4Addr, void (*)(RsIpv4Addr *)> rimpl_;
};

} // namespace testcontainers
