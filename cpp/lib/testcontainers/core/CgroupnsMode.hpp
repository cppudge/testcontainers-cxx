#pragma once

#include <memory>

#include "testcontainers/interfaces/IRustObject.hpp"

class RsCgroupnsMode;

namespace testcontainers {

class CgroupnsMode final : public IRustObject {
public: // Static factory methods
  static CgroupnsMode Private() noexcept;
  static CgroupnsMode Host() noexcept;

public: // Default construction methods
  CgroupnsMode(CgroupnsMode &&other) noexcept;
  CgroupnsMode &operator=(CgroupnsMode &&other) noexcept;
  ~CgroupnsMode() noexcept;
  CgroupnsMode(const CgroupnsMode &) = delete;
  CgroupnsMode &operator=(const CgroupnsMode &) = delete;

public: // IRustObject interface
  bool is_valid() const noexcept override;

private:
  friend class GenericImage;
  friend class ContainerRequest;
  friend class Container;

  explicit CgroupnsMode(RsCgroupnsMode *mode) noexcept;

private:
  std::unique_ptr<RsCgroupnsMode, void (*)(RsCgroupnsMode *)> rimpl_;
};

} // namespace testcontainers
