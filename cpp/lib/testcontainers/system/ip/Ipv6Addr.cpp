#include <rust/cxx.h>
#include <rust_tc_bridge/lib.h>

#include "testcontainers/system/ip/IpAddr.hpp"
#include "testcontainers/system/ip/Ipv6Addr.hpp"


#include "details/BoxHelper.hpp"

namespace testcontainers {

Ipv6Addr::Ipv6Addr(RsIpv6Addr *addr) noexcept
    : rimpl_(addr, [](RsIpv6Addr *a) { ::rs_ipv6_addr_destroy(details::box_from_raw(a)); }) {}

Ipv6Addr::Ipv6Addr(std::uint16_t a, std::uint16_t b, std::uint16_t c, std::uint16_t d,
                   std::uint16_t e, std::uint16_t f, std::uint16_t g, std::uint16_t h) noexcept
    : Ipv6Addr(::rs_ipv6_addr_new(a, b, c, d, e, f, g, h).into_raw()) {}

Ipv6Addr::Ipv6Addr(Ipv6Addr &&other) noexcept = default;

Ipv6Addr &Ipv6Addr::operator=(Ipv6Addr &&other) noexcept = default;

Ipv6Addr::~Ipv6Addr() noexcept = default;

Ipv6Addr Ipv6Addr::from_bits(const std::uint64_t bits_low, const std::uint64_t bits_high) noexcept {
  return Ipv6Addr(::rs_ipv6_addr_from_bits(bits_low, bits_high).into_raw());
}

Ipv6Addr Ipv6Addr::localhost() noexcept { return Ipv6Addr(::rs_ipv6_addr_localhost().into_raw()); }

Ipv6Addr Ipv6Addr::unspecified() noexcept {
  return Ipv6Addr(::rs_ipv6_addr_unspecified().into_raw());
}

Ipv6Addr Ipv6Addr::from_segments(const std::array<std::uint16_t, 8> &segments) noexcept {
  return Ipv6Addr(::rs_ipv6_addr_from_segments(segments).into_raw());
}

Ipv6Addr Ipv6Addr::from_octets(const std::array<std::uint8_t, 16> &octets) noexcept {
  return Ipv6Addr(::rs_ipv6_addr_from_octets(octets).into_raw());
}

std::pair<std::uint64_t, std::uint64_t> Ipv6Addr::to_bits() const noexcept {
  auto bits_pair = rimpl_->rs_ipv6_addr_to_bits_pair();
  return {bits_pair[0], bits_pair[1]};
}

std::array<std::uint16_t, 8> Ipv6Addr::segments() const noexcept {
  return rimpl_->rs_ipv6_addr_segments();
}

std::array<std::uint8_t, 16> Ipv6Addr::octets() const noexcept {
  return rimpl_->rs_ipv6_addr_octets();
}

bool Ipv6Addr::is_unspecified() const noexcept { return rimpl_->rs_ipv6_addr_is_unspecified(); }

bool Ipv6Addr::is_loopback() const noexcept { return rimpl_->rs_ipv6_addr_is_loopback(); }

bool Ipv6Addr::is_unicast_link_local() const noexcept {
  return rimpl_->rs_ipv6_addr_is_unicast_link_local();
}

bool Ipv6Addr::is_unique_local() const noexcept { return rimpl_->rs_ipv6_addr_is_unique_local(); }

bool Ipv6Addr::is_multicast() const noexcept { return rimpl_->rs_ipv6_addr_is_multicast(); }

IpAddr Ipv6Addr::to_canonical() const noexcept {
  return IpAddr(rimpl_->rs_ipv6_addr_to_canonical().into_raw());
}

std::string Ipv6Addr::to_string() const noexcept {
  return std::string(rimpl_->rs_ipv6_addr_to_string());
}

bool Ipv6Addr::is_valid() const noexcept { return static_cast<bool>(rimpl_); }

} // namespace testcontainers
