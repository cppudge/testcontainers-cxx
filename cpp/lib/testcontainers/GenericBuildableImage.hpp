#pragma once

#include <filesystem>
#include <memory>
#include <string_view>
#include <vector>

#include "testcontainers/interfaces/IBuildableImage.hpp"
#include "testcontainers/interfaces/IRustObject.hpp"
#include "testcontainers/interfaces/ISyncBuilder.hpp"

class RsGenericBuildableImage;

namespace testcontainers {

class GenericImage;

class GenericBuildableImage final : public IRustObject,
                                    public IBuildableImage,
                                    public ISyncBuilder {
public: // Static factory methods
  explicit GenericBuildableImage(std::string_view name, std::string_view tag);

public: // Default construction methods
  GenericBuildableImage(GenericBuildableImage &&other) noexcept;
  GenericBuildableImage &operator=(GenericBuildableImage &&other) noexcept;
  ~GenericBuildableImage() noexcept;
  GenericBuildableImage(const GenericBuildableImage &) = delete;
  GenericBuildableImage &operator=(const GenericBuildableImage &) = delete;

public: // IRustObject interface
  bool is_valid() const noexcept override;

public: // IBuildableImage interface
  GenericBuildableImage with_dockerfile(const std::filesystem::path &source) override;
  GenericBuildableImage with_dockerfile_string(std::string_view content) override;
  GenericBuildableImage with_file(const std::filesystem::path &source,
                                  std::string_view target) override;
  GenericBuildableImage with_data(const std::vector<std::uint8_t> &data,
                                  std::string_view target) override;

public: // ISyncBuilder interface
  GenericImage build() override;

private:
  explicit GenericBuildableImage(RsGenericBuildableImage *image) noexcept;

private:
  std::unique_ptr<RsGenericBuildableImage, void (*)(RsGenericBuildableImage *)> rimpl_;
};

} // namespace testcontainers
