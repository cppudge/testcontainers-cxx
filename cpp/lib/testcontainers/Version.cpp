#include "testcontainers/Version.hpp"

#include <rust/cxx.h>
#include <rust_tc_bridge/lib.h>

namespace testcontainers {

std::string version() { return std::string(::version()); }

} // namespace testcontainers
