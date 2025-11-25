#pragma once

#include <exception>
#include <string>

namespace testcontainers {

class Error final : public std::exception {
public:
  explicit Error(std::string message);

  Error(const Error &other) = default;
  Error(Error &&other) noexcept = default;
  Error &operator=(const Error &other) = default;
  Error &operator=(Error &&other) noexcept = default;
  ~Error() = default;

public: // Exception interface
  const char *what() const noexcept override { return message_.c_str(); }

private:
  std::string message_;
};

} // namespace testcontainers
