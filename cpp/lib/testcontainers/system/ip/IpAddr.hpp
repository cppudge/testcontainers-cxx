#pragma once

#include <memory>
#include <string>

#include "testcontainers/interfaces/IRustObject.hpp"

class RsIpAddr;

namespace testcontainers {

class Ipv4Addr;
class Ipv6Addr;

class IpAddr final : public IRustObject {
public:
  static IpAddr from_ipv4(Ipv4Addr ipv4) noexcept;
  static IpAddr from_ipv6(Ipv6Addr ipv6) noexcept;

  bool is_unspecified() const noexcept;
  bool is_loopback() const noexcept;
  bool is_multicast() const noexcept;
  bool is_ipv4() const noexcept;
  bool is_ipv6() const noexcept;

  IpAddr to_canonical() const noexcept;
  Ipv4Addr to_ipv4() const;
  Ipv6Addr to_ipv6() const;
  std::string to_string() const noexcept;

public:
  IpAddr(IpAddr &&other) noexcept;
  IpAddr &operator=(IpAddr &&other) noexcept;
  ~IpAddr() noexcept;
  IpAddr(const IpAddr &) = delete;
  IpAddr &operator=(const IpAddr &) = delete;

public:
  bool is_valid() const noexcept override;

private:
  friend class Ipv6Addr;

  explicit IpAddr(RsIpAddr *addr) noexcept;

private:
  std::unique_ptr<RsIpAddr, void (*)(RsIpAddr *)> rimpl_;
};

} // namespace testcontainers
