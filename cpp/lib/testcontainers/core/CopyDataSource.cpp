#include <rust/cxx.h>
#include <rust_tc_bridge/lib.h>

#include "testcontainers/core/CopyDataSource.hpp"

#include "details/BoxHelper.hpp"
#include "testcontainers/system/Path.hpp"

namespace testcontainers {

CopyDataSource::CopyDataSource(RsCopyDataSource *source) noexcept
    : rimpl_(source, [](RsCopyDataSource *s) {
        ::rs_copy_data_source_destroy(details::box_from_raw(s));
      }) {}

CopyDataSource::CopyDataSource(CopyDataSource &&other) noexcept = default;

CopyDataSource &CopyDataSource::operator=(CopyDataSource &&other) noexcept = default;

CopyDataSource::~CopyDataSource() noexcept = default;

CopyDataSource CopyDataSource::File(const std::filesystem::path &path) noexcept {
  Path path_obj(path);
  return CopyDataSource(
      ::rs_copy_data_source_file(details::into_box(path_obj.rimpl_)).into_raw());
}

CopyDataSource CopyDataSource::Data(std::vector<std::uint8_t> data) noexcept {
  rust::Vec<std::uint8_t> rust_data;
  rust_data.reserve(data.size());
  for (const auto byte : data) {
    rust_data.push_back(byte);
  }
  return CopyDataSource(::rs_copy_data_source_data(std::move(rust_data)).into_raw());
}

bool CopyDataSource::is_valid() const noexcept { return static_cast<bool>(rimpl_); }

} // namespace testcontainers
