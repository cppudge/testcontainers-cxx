#include <rust/cxx.h>
#include <rust_tc_bridge/lib.h>

#include "testcontainers/system/ip/IpAddr.hpp"
#include "testcontainers/system/ip/Ipv4Addr.hpp"
#include "testcontainers/system/ip/Ipv6Addr.hpp"

#include "details/BoxHelper.hpp"
#include "details/ErrorHelper.hpp"

namespace testcontainers {

IpAddr::IpAddr(RsIpAddr *addr) noexcept
    : rimpl_(addr, [](RsIpAddr *a) { ::rs_ip_addr_destroy(details::box_from_raw(a)); }) {}

IpAddr::IpAddr(IpAddr &&other) noexcept = default;

IpAddr &IpAddr::operator=(IpAddr &&other) noexcept = default;

IpAddr::~IpAddr() noexcept = default;

IpAddr IpAddr::from_ipv4(Ipv4Addr ipv4) noexcept {
  return IpAddr(::rs_ip_addr_from_ipv4(details::into_box(ipv4.rimpl_)).into_raw());
}

IpAddr IpAddr::from_ipv6(Ipv6Addr ipv6) noexcept {
  return IpAddr(::rs_ip_addr_from_ipv6(details::into_box(ipv6.rimpl_)).into_raw());
}

bool IpAddr::is_unspecified() const noexcept { return rimpl_->rs_ip_addr_is_unspecified(); }

bool IpAddr::is_loopback() const noexcept { return rimpl_->rs_ip_addr_is_loopback(); }

bool IpAddr::is_multicast() const noexcept { return rimpl_->rs_ip_addr_is_multicast(); }

bool IpAddr::is_ipv4() const noexcept { return rimpl_->rs_ip_addr_is_ipv4(); }

bool IpAddr::is_ipv6() const noexcept { return rimpl_->rs_ip_addr_is_ipv6(); }

IpAddr IpAddr::to_canonical() const noexcept {
  return IpAddr(rimpl_->rs_ip_addr_to_canonical().into_raw());
}

Ipv4Addr IpAddr::to_ipv4() const {
  return Ipv4Addr(details::call_map_error([&] { return rimpl_->rs_ip_addr_to_ipv4(); }).into_raw());
}

Ipv6Addr IpAddr::to_ipv6() const {
  return Ipv6Addr(details::call_map_error([&] { return rimpl_->rs_ip_addr_to_ipv6(); }).into_raw());
}

std::string IpAddr::to_string() const noexcept { return std::string(rimpl_->rs_ip_addr_to_string()); }

bool IpAddr::is_valid() const noexcept { return static_cast<bool>(rimpl_); }

} // namespace testcontainers
