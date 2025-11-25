#pragma once

#include <array>
#include <cstdint>
#include <memory>
#include <string>
#include <utility>

#include "testcontainers/interfaces/IRustObject.hpp"

class RsIpv6Addr;

namespace testcontainers {

class IpAddr;

class Ipv6Addr final : public IRustObject {
public:
  static constexpr std::uint32_t BITS = 128;

  explicit Ipv6Addr(std::uint16_t a, std::uint16_t b, std::uint16_t c, std::uint16_t d,
                    std::uint16_t e, std::uint16_t f, std::uint16_t g, std::uint16_t h) noexcept;

  // MSVC does not support std::uint128_t, so we use two std::uint64_t instead.
  static Ipv6Addr from_bits(const std::uint64_t bits_low, const std::uint64_t bits_high) noexcept;
  static Ipv6Addr localhost() noexcept;
  static Ipv6Addr unspecified() noexcept;
  static Ipv6Addr from_segments(const std::array<std::uint16_t, 8> &segments) noexcept;
  static Ipv6Addr from_octets(const std::array<std::uint8_t, 16> &octets) noexcept;

  bool is_unspecified() const noexcept;
  bool is_loopback() const noexcept;
  bool is_unicast_link_local() const noexcept;
  bool is_unique_local() const noexcept;
  bool is_multicast() const noexcept;

  std::pair<std::uint64_t, std::uint64_t> to_bits() const noexcept;
  std::array<std::uint16_t, 8> segments() const noexcept;
  std::array<std::uint8_t, 16> octets() const noexcept;
  IpAddr to_canonical() const noexcept;
  std::string to_string() const noexcept;

public:
  Ipv6Addr(Ipv6Addr &&other) noexcept;
  Ipv6Addr &operator=(Ipv6Addr &&other) noexcept;
  ~Ipv6Addr() noexcept;
  Ipv6Addr(const Ipv6Addr &) = delete;
  Ipv6Addr &operator=(const Ipv6Addr &) = delete;

public:
  bool is_valid() const noexcept override;

private:
  friend class IpAddr;
  friend class UrlHost;

  explicit Ipv6Addr(RsIpv6Addr *addr) noexcept;

private:
  std::unique_ptr<RsIpv6Addr, void (*)(RsIpv6Addr *)> rimpl_;
};

} // namespace testcontainers
