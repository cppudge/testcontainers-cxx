#pragma once

#include <filesystem>
#include <memory>
#include <vector>

#include "testcontainers/interfaces/IRustObject.hpp"

class RsCopyDataSource;

namespace testcontainers {

class CopyDataSource final : public IRustObject {
public: // Static factory methods
  static CopyDataSource File(const std::filesystem::path &path) noexcept;
  static CopyDataSource Data(std::vector<std::uint8_t> data) noexcept;

public: // Default construction methods
  CopyDataSource(CopyDataSource &&other) noexcept;
  CopyDataSource &operator=(CopyDataSource &&other) noexcept;
  ~CopyDataSource() noexcept;
  CopyDataSource(const CopyDataSource &) = delete;
  CopyDataSource &operator=(const CopyDataSource &) = delete;

public: // IRustObject interface
  bool is_valid() const noexcept override;

private:
  friend class GenericImage;
  friend class ContainerRequest;

  explicit CopyDataSource(RsCopyDataSource *source) noexcept;

private:
  std::unique_ptr<RsCopyDataSource, void (*)(RsCopyDataSource *)> rimpl_;
};

} // namespace testcontainers
