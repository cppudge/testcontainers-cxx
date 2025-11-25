#pragma once

#include <filesystem>
#include <memory>
#include <optional>
#include <string>

#include "testcontainers/interfaces/IRustObject.hpp"

class RsPath;

namespace testcontainers {

class Path final : public IRustObject {
public:
  explicit Path(const std::filesystem::path &p) noexcept;

  Path(Path &&other) noexcept;
  Path &operator=(Path &&other) noexcept;
  ~Path() noexcept;
  Path(const Path &) = delete;
  Path &operator=(const Path &) = delete;

public:
  bool is_valid() const noexcept override;

  std::optional<std::string> to_string() const;

private:
  friend class CopyDataSource;
  friend class GenericBuildableImage;

  std::unique_ptr<RsPath, void (*)(RsPath *)> rimpl_;
};

} // namespace testcontainers
