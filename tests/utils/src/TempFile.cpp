#include "testutils/TempFile.hpp"

namespace testcontainers {
namespace test_utils {

// Initialize static counter
std::atomic<int> TempFile::counter_{0};

} // namespace test_utils
} // namespace testcontainers

