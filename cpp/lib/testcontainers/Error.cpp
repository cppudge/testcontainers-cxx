#include "testcontainers/Error.hpp"

namespace testcontainers {

Error::Error(std::string message) : message_(std::move(message)) {}
} // namespace testcontainers
