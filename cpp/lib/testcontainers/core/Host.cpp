#include <rust/cxx.h>
#include <rust_tc_bridge/lib.h>

#include "testcontainers/core/Host.hpp"

#include "details/BoxHelper.hpp"
#include "details/ErrorHelper.hpp"

namespace testcontainers {

Host::Host(RsHost *host) noexcept
    : rimpl_(host, [](RsHost *host) { ::rs_host_destroy(details::box_from_raw(host)); }) {}

Host::Host(Host &&other) noexcept = default;

Host &Host::operator=(Host &&other) noexcept = default;

Host::~Host() noexcept = default;

Host Host::Addr(std::string_view ip_addr) {
  return Host(details::call_map_error(::rs_host_addr, details::into_string(ip_addr)).into_raw());
}

Host Host::HostGatewayLinux() noexcept {
  return Host(::rs_host_host_gateway_linux().into_raw());
}

bool Host::is_valid() const noexcept { return static_cast<bool>(rimpl_); }

} // namespace testcontainers
