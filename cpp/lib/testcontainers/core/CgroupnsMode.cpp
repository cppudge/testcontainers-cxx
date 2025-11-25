#include <rust/cxx.h>
#include <rust_tc_bridge/lib.h>

#include "testcontainers/core/CgroupnsMode.hpp"

#include "details/BoxHelper.hpp"

namespace testcontainers {

CgroupnsMode::CgroupnsMode(RsCgroupnsMode *mode) noexcept
    : rimpl_(mode,
             [](RsCgroupnsMode *m) { ::rs_cgroupns_mode_destroy(details::box_from_raw(m)); }) {}

CgroupnsMode::CgroupnsMode(CgroupnsMode &&other) noexcept = default;

CgroupnsMode &CgroupnsMode::operator=(CgroupnsMode &&other) noexcept = default;

CgroupnsMode::~CgroupnsMode() noexcept = default;

CgroupnsMode CgroupnsMode::Private() noexcept {
  return CgroupnsMode(::rs_cgroupns_mode_private().into_raw());
}

CgroupnsMode CgroupnsMode::Host() noexcept {
  return CgroupnsMode(::rs_cgroupns_mode_host().into_raw());
}

bool CgroupnsMode::is_valid() const noexcept { return static_cast<bool>(rimpl_); }

} // namespace testcontainers
