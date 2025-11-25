#pragma once

#include <string>

namespace testcontainers {

/**
 * @brief Returns the version of the testcontainers-cxx bridge.
 *
 * This version is retrieved from the underlying Rust implementation.
 *
 * @return Version string (e.g., "0.1.0")
 */
std::string version();

} // namespace testcontainers
