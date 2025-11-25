#include <rust/cxx.h>
#include <rust_tc_bridge/lib.h>

#include "testcontainers/system/UrlHost.hpp"
#include "testcontainers/system/ip/Ipv4Addr.hpp"
#include "testcontainers/system/ip/Ipv6Addr.hpp"

#include "details/BoxHelper.hpp"
#include "details/ErrorHelper.hpp"

namespace testcontainers {

UrlHost::UrlHost(RsUrlHost *host) noexcept
    : rimpl_(host, [](RsUrlHost *h) { ::rs_url_host_destroy(details::box_from_raw(h)); }) {}

UrlHost::UrlHost(UrlHost &&other) noexcept = default;

UrlHost &UrlHost::operator=(UrlHost &&other) noexcept = default;

UrlHost::~UrlHost() noexcept = default;

UrlHost UrlHost::domain(std::string_view domain) {
  return UrlHost(
      details::call_map_error([&] { return ::rs_url_host_domain(details::into_string(domain)); }).into_raw());
}

UrlHost UrlHost::from_ipv4(Ipv4Addr ipv4) noexcept {
  return UrlHost(::rs_url_host_from_ipv4(details::into_box(ipv4.rimpl_)).into_raw());
}

UrlHost UrlHost::from_ipv6(Ipv6Addr ipv6) noexcept {
  return UrlHost(::rs_url_host_from_ipv6(details::into_box(ipv6.rimpl_)).into_raw());
}

bool UrlHost::is_domain() const noexcept { return rimpl_->rs_url_host_is_domain(); }

bool UrlHost::is_ipv4() const noexcept { return rimpl_->rs_url_host_is_ipv4(); }

bool UrlHost::is_ipv6() const noexcept { return rimpl_->rs_url_host_is_ipv6(); }

std::string UrlHost::to_domain() const {
  return std::string(details::call_map_error([&] { return rimpl_->rs_url_host_to_domain(); }));
}

Ipv4Addr UrlHost::to_ipv4() const {
  return Ipv4Addr(
      details::call_map_error([&] { return rimpl_->rs_url_host_to_ipv4(); }).into_raw());
}

Ipv6Addr UrlHost::to_ipv6() const {
  return Ipv6Addr(
      details::call_map_error([&] { return rimpl_->rs_url_host_to_ipv6(); }).into_raw());
}

std::string UrlHost::to_string() const noexcept { return std::string(rimpl_->rs_url_host_to_string()); }

bool UrlHost::is_valid() const noexcept { return static_cast<bool>(rimpl_); }

} // namespace testcontainers
