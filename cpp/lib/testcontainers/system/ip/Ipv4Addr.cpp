#include <rust/cxx.h>
#include <rust_tc_bridge/lib.h>

#include "testcontainers/system/ip/Ipv4Addr.hpp"

#include "details/BoxHelper.hpp"

namespace testcontainers {

Ipv4Addr::Ipv4Addr(RsIpv4Addr *addr) noexcept
    : rimpl_(addr, [](RsIpv4Addr *a) { ::rs_ipv4_addr_destroy(details::box_from_raw(a)); }) {}

Ipv4Addr::Ipv4Addr(std::uint8_t a, std::uint8_t b, std::uint8_t c, std::uint8_t d) noexcept
    : Ipv4Addr(::rs_ipv4_addr_new(a, b, c, d).into_raw()) {}

Ipv4Addr::Ipv4Addr(Ipv4Addr &&other) noexcept = default;

Ipv4Addr &Ipv4Addr::operator=(Ipv4Addr &&other) noexcept = default;

Ipv4Addr::~Ipv4Addr() noexcept = default;

Ipv4Addr Ipv4Addr::from_bits(std::uint32_t bits) noexcept {
  return Ipv4Addr(::rs_ipv4_addr_from_bits(bits).into_raw());
}

Ipv4Addr Ipv4Addr::localhost() noexcept { return Ipv4Addr(::rs_ipv4_addr_localhost().into_raw()); }

Ipv4Addr Ipv4Addr::unspecified() noexcept { return Ipv4Addr(::rs_ipv4_addr_unspecified().into_raw()); }

Ipv4Addr Ipv4Addr::broadcast() noexcept { return Ipv4Addr(::rs_ipv4_addr_broadcast().into_raw()); }

Ipv4Addr Ipv4Addr::from_octets(const std::array<std::uint8_t, 4> &octets) noexcept {
  return Ipv4Addr(::rs_ipv4_addr_from_octets(octets).into_raw());
}

std::uint32_t Ipv4Addr::to_bits() const noexcept { return rimpl_->rs_ipv4_addr_to_bits(); }

std::array<std::uint8_t, 4> Ipv4Addr::octets() const noexcept { return rimpl_->rs_ipv4_addr_octets(); }

bool Ipv4Addr::is_unspecified() const noexcept { return rimpl_->rs_ipv4_addr_is_unspecified(); }

bool Ipv4Addr::is_loopback() const noexcept { return rimpl_->rs_ipv4_addr_is_loopback(); }

bool Ipv4Addr::is_private() const noexcept { return rimpl_->rs_ipv4_addr_is_private(); }

bool Ipv4Addr::is_link_local() const noexcept { return rimpl_->rs_ipv4_addr_is_link_local(); }

bool Ipv4Addr::is_multicast() const noexcept { return rimpl_->rs_ipv4_addr_is_multicast(); }

bool Ipv4Addr::is_broadcast() const noexcept { return rimpl_->rs_ipv4_addr_is_broadcast(); }

bool Ipv4Addr::is_documentation() const noexcept { return rimpl_->rs_ipv4_addr_is_documentation(); }

std::string Ipv4Addr::to_string() const noexcept { return std::string(rimpl_->rs_ipv4_addr_to_string()); }

bool Ipv4Addr::is_valid() const noexcept { return static_cast<bool>(rimpl_); }

} // namespace testcontainers
