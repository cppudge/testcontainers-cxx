#pragma once

#include <memory>
#include <string>

#include "testcontainers/interfaces/IRustObject.hpp"

class RsUrlHost;

namespace testcontainers {

class Ipv4Addr;
class Ipv6Addr;

class UrlHost final : public IRustObject {
public:
  static UrlHost domain(std::string_view domain);

  static UrlHost from_ipv4(Ipv4Addr ipv4) noexcept;

  static UrlHost from_ipv6(Ipv6Addr ipv6) noexcept;

  bool is_domain() const noexcept;
  bool is_ipv4() const noexcept;
  bool is_ipv6() const noexcept;

  std::string to_domain() const;

  Ipv4Addr to_ipv4() const;

  Ipv6Addr to_ipv6() const;

  std::string to_string() const noexcept;

public:
  UrlHost(UrlHost &&other) noexcept;
  UrlHost &operator=(UrlHost &&other) noexcept;
  ~UrlHost() noexcept;
  UrlHost(const UrlHost &) = delete;
  UrlHost &operator=(const UrlHost &) = delete;

public:
  bool is_valid() const noexcept override;

private:
  friend class Host;
  friend class Container;

  explicit UrlHost(RsUrlHost *host) noexcept;

private:
  std::unique_ptr<RsUrlHost, void (*)(RsUrlHost *)> rimpl_;
};

} // namespace testcontainers
