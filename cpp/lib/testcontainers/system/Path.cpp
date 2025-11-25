#include <type_traits>

#include <rust/cxx.h>
#include <rust_tc_bridge/lib.h>

#include "testcontainers/system/Path.hpp"

#include "details/BoxHelper.hpp"
#include "details/ErrorHelper.hpp"
#include "details/OptionHelper.hpp"

namespace testcontainers {

namespace {
template <typename T> struct always_false : std::false_type {};

RsPath *platform_specific_construct(const std::filesystem::path &p) noexcept {
  using V = std::filesystem::path::value_type;

  static_assert(std::is_same_v<V, wchar_t> || std::is_same_v<V, char>,
                "unsupported std::filesystem::path::value_type");

  if constexpr (std::is_same_v<V, wchar_t>) {
    const auto &w = p.native();
    return ::rs_path_from_utf16(rust::Slice<const std::uint16_t>(
                                     reinterpret_cast<const std::uint16_t *>(w.data()), w.size()))
        .into_raw();
  } else {
    const auto &s = p.native();
    return ::rs_path_from_bytes(rust::Slice<const std::uint8_t>(
                                     reinterpret_cast<const std::uint8_t *>(s.data()), s.size()))
        .into_raw();
  }
}
} // namespace

Path::Path(const std::filesystem::path &p) noexcept
    : rimpl_(platform_specific_construct(p),
             [](RsPath *p) { ::rs_path_destroy(details::box_from_raw(p)); }) {}

Path::Path(Path &&other) noexcept = default;

Path &Path::operator=(Path &&other) noexcept = default;

Path::~Path() noexcept = default;

bool Path::is_valid() const noexcept { return static_cast<bool>(rimpl_); }

std::optional<std::string> Path::to_string() const {
  return utils::vec_to_optional<std::string>(
      details::call_map_error([&] { return rimpl_->rs_path_to_string_opt(); }));
}

} // namespace testcontainers
