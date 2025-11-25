# Testcontainers Test Utilities

Shared utility library for testcontainers-cxx unit and integration tests.

## Contents

### TempFile
RAII helper class for creating temporary test files.

**Usage:**
```cpp
#include "testutils/TempFile.hpp"

using testcontainers::test_utils::TempFile;

// Create temporary file with content
TempFile config("server_port=8080\n");

// Get file path
auto path = config.path();

// File is automatically deleted when TempFile goes out of scope
```

### DockerCli
Docker CLI helper functions for image management.

**Usage:**
```cpp
#include "testutils/DockerCli.hpp"

using testcontainers::test_utils::DockerCli;

// Check if Docker is available
if (DockerCli::is_docker_available()) {
    // Remove all images by name
    DockerCli::remove_images_by_name("my-test-image");
    
    // Remove specific image by name:tag
    DockerCli::remove_image("my-test-image", "latest");
}
```

### Convenience Header
Include all utilities at once:

```cpp
#include "testutils/testutils.hpp"

// Now you can use both TempFile and DockerCli
```

## Building

The test utilities are automatically built as part of the test suite:

```bash
cmake --build build/native --config Debug --target testcontainers_test_utils
```

## Dependencies

- C++17 or later
- Standard library only (no external dependencies)
- Platform-specific:
  - Windows: Uses `_popen` and `_pclose`
  - Unix/Linux: Uses `popen` and `pclose`
