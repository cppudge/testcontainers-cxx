#include <rust/cxx.h>
#include <rust_tc_bridge/lib.h>

#include "testcontainers/core/Mount.hpp"

#include "details/BoxHelper.hpp"

namespace testcontainers {

Mount::Mount(RsMount *mount) noexcept
    : rimpl_(mount, [](RsMount *m) { ::rs_mount_destroy(details::box_from_raw(m)); }) {}

Mount::Mount(Mount &&other) noexcept = default;

Mount &Mount::operator=(Mount &&other) noexcept = default;

Mount::~Mount() noexcept = default;

Mount Mount::BindMount(std::string_view host_path, std::string_view container_path) noexcept {
  return Mount(::rs_mount_bind_mount(details::into_string(host_path), details::into_string(container_path)).into_raw());
}

Mount Mount::VolumeMount(std::string_view name, std::string_view container_path) noexcept {
  return Mount(::rs_mount_volume_mount(details::into_string(name), details::into_string(container_path)).into_raw());
}

Mount Mount::TmpfsMount(std::string_view container_path) noexcept {
  return Mount(::rs_mount_tmpfs_mount(details::into_string(container_path)).into_raw());
}

Mount Mount::with_read_only() noexcept {
  return Mount(::rs_mount_with_read_only(details::into_box(rimpl_)).into_raw());
}

Mount Mount::with_read_write() noexcept {
  return Mount(::rs_mount_with_read_write(details::into_box(rimpl_)).into_raw());
}

Mount Mount::with_size_bytes(std::int64_t size) noexcept {
  return Mount(::rs_mount_with_size_bytes(details::into_box(rimpl_), size).into_raw());
}

Mount Mount::with_mode(std::int64_t mode) noexcept {
  return Mount(::rs_mount_with_mode(details::into_box(rimpl_), mode).into_raw());
}

bool Mount::is_valid() const noexcept { return static_cast<bool>(rimpl_); }

} // namespace testcontainers
