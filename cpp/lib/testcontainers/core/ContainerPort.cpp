#include <rust/cxx.h>
#include <rust_tc_bridge/lib.h>

#include "testcontainers/core/ContainerPort.hpp"

#include "details/BoxHelper.hpp"

namespace testcontainers {

ContainerPort::ContainerPort(RsContainerPort *rimpl) noexcept
    : rimpl_(rimpl,
             [](RsContainerPort *p) { ::rs_container_port_destroy(details::box_from_raw(p)); }) {}

ContainerPort::ContainerPort(ContainerPort &&other) noexcept = default;

ContainerPort &ContainerPort::operator=(ContainerPort &&other) noexcept = default;

ContainerPort::~ContainerPort() noexcept = default;

ContainerPort ContainerPort::Tcp(std::uint16_t port) noexcept {
  return ContainerPort(::rs_container_port_tcp(port).into_raw());
}

ContainerPort ContainerPort::Udp(uint16_t port) noexcept {
  return ContainerPort(::rs_container_port_udp(port).into_raw());
}

ContainerPort ContainerPort::Sctp(uint16_t port) noexcept {
  return ContainerPort(::rs_container_port_sctp(port).into_raw());
}

bool ContainerPort::is_valid() const noexcept { return static_cast<bool>(rimpl_); }

std::uint16_t ContainerPort::as_u16() const noexcept {
  return rimpl_->rs_container_port_as_u16();
}

bool ContainerPort::is_tcp() const noexcept { return rimpl_->rs_container_port_is_tcp(); }
bool ContainerPort::is_udp() const noexcept { return rimpl_->rs_container_port_is_udp(); }
bool ContainerPort::is_sctp() const noexcept { return rimpl_->rs_container_port_is_sctp(); }

} // namespace testcontainers
