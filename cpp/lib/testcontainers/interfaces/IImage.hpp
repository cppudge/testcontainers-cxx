#pragma once

#include <string_view>
namespace testcontainers {

class GenericImage;
class ContainerPort;
class WaitFor;

class IImage {
public:
  virtual ~IImage() = default;

  virtual GenericImage with_exposed_port(ContainerPort port) noexcept = 0;
  virtual GenericImage with_entrypoint(std::string_view entrypoint) = 0;
  virtual GenericImage with_wait_for(WaitFor wait_for) noexcept = 0;
};

} // namespace testcontainers
