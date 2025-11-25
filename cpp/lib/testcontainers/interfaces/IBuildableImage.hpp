#pragma once

#include <filesystem>
#include <vector>

namespace testcontainers {

class GenericBuildableImage;

class IBuildableImage {
public:
  virtual ~IBuildableImage() = default;

  virtual GenericBuildableImage with_dockerfile(const std::filesystem::path &source) = 0;
  virtual GenericBuildableImage with_dockerfile_string(std::string_view content) = 0;
  virtual GenericBuildableImage with_file(const std::filesystem::path &source, std::string_view target) = 0;
  virtual GenericBuildableImage with_data(const std::vector<std::uint8_t> &data, std::string_view target) = 0;
};

} // namespace testcontainers
