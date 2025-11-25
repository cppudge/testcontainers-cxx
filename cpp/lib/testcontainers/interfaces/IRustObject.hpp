#pragma once

namespace testcontainers {

class IRustObject {
public:
  virtual ~IRustObject() = default;
  virtual bool is_valid() const noexcept = 0;
  // TODO: clone method
};

} // namespace testcontainers
