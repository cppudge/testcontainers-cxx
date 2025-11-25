#pragma once

namespace testcontainers {

class Container;
class ContainerRequest;

class ISyncRunner {
public:
  virtual ~ISyncRunner() = default;

  virtual Container start() = 0;
  virtual ContainerRequest pull() = 0;
};

} // namespace testcontainers
