#pragma once

#include <cstdint>
#include <memory>
#include <string_view>

#include "testcontainers/interfaces/IRustObject.hpp"

class RsMount;

namespace testcontainers {

class Mount final : public IRustObject {
public: // Static factory methods
  static Mount BindMount(std::string_view host_path, std::string_view container_path) noexcept;
  static Mount VolumeMount(std::string_view name, std::string_view container_path) noexcept;
  static Mount TmpfsMount(std::string_view container_path) noexcept;

public: // Non-static factory methods
  Mount with_read_only() noexcept;
  Mount with_read_write() noexcept;
  Mount with_size_bytes(std::int64_t size) noexcept;
  Mount with_mode(std::int64_t mode) noexcept;

public: // Default construction methods
  Mount(Mount &&other) noexcept;
  Mount &operator=(Mount &&other) noexcept;
  ~Mount() noexcept;
  Mount(const Mount &) = delete;
  Mount &operator=(const Mount &) = delete;

public: // IRustObject interface
  bool is_valid() const noexcept override;

private:
  friend class GenericImage;
  friend class ContainerRequest;

  explicit Mount(RsMount *mount) noexcept;

private:
  std::unique_ptr<RsMount, void (*)(RsMount *)> rimpl_;
};

} // namespace testcontainers
