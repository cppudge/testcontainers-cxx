#include <rust/cxx.h>
#include <rust_tc_bridge/lib.h>

#include "details/VectorHelper.hpp"
#include "testcontainers/GenericBuildableImage.hpp"
#include "testcontainers/GenericImage.hpp"
#include "testcontainers/system/Path.hpp"

#include "details/BoxHelper.hpp"
#include "details/ErrorHelper.hpp"

namespace testcontainers {

GenericBuildableImage::GenericBuildableImage(RsGenericBuildableImage *image) noexcept
    : rimpl_(image, [](RsGenericBuildableImage *img) {
        ::rs_generic_buildable_image_destroy(details::box_from_raw(img));
      }) {}

GenericBuildableImage::GenericBuildableImage(std::string_view name, std::string_view tag)
    : GenericBuildableImage(
          ::rs_generic_buildable_image_new(details::into_string(name), details::into_string(tag))
              .into_raw()) {}

GenericBuildableImage::GenericBuildableImage(GenericBuildableImage &&other) noexcept = default;

GenericBuildableImage &GenericBuildableImage::operator=(GenericBuildableImage &&other) noexcept
    = default;

GenericBuildableImage::~GenericBuildableImage() noexcept = default;

bool GenericBuildableImage::is_valid() const noexcept { return static_cast<bool>(rimpl_); }

GenericBuildableImage GenericBuildableImage::with_dockerfile(const std::filesystem::path &source) {
  Path path_obj(source);
  return GenericBuildableImage(
      ::rs_generic_buildable_image_with_dockerfile(details::into_box(rimpl_),
                                                   details::into_box(path_obj.rimpl_))
          .into_raw());
}

GenericBuildableImage GenericBuildableImage::with_dockerfile_string(std::string_view content) {
  return GenericBuildableImage(::rs_generic_buildable_image_with_dockerfile_string(
                                   details::into_box(rimpl_), details::into_string(content))
                                   .into_raw());
}

GenericBuildableImage GenericBuildableImage::with_file(const std::filesystem::path &source,
                                                       std::string_view target) {
  Path path_obj(source);
  return GenericBuildableImage(
      ::rs_generic_buildable_image_with_file(details::into_box(rimpl_),
                                             details::into_box(path_obj.rimpl_),
                                             details::into_string(target))
          .into_raw());
}

GenericBuildableImage GenericBuildableImage::with_data(const std::vector<std::uint8_t> &data,
                                                       std::string_view target) {
  return GenericBuildableImage(
      ::rs_generic_buildable_image_with_data(details::into_box(rimpl_),
                                             utils::vector_to_vec(std::move(data)),
                                             details::into_string(target))
          .into_raw());
}

GenericImage GenericBuildableImage::build() {
  return GenericImage(
      details::call_map_error(::rs_generic_buildable_image_build, details::into_box(rimpl_))
          .into_raw());
}

} // namespace testcontainers
