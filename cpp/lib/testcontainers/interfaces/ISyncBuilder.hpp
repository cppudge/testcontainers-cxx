#pragma once

namespace testcontainers {

class GenericImage;

class ISyncBuilder {
public:
  virtual ~ISyncBuilder() = default;

  virtual GenericImage build() = 0;
};

} // namespace testcontainers
