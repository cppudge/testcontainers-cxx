# Integration Tests

Integration tests for testcontainers-cxx that require a running Docker daemon.

## Prerequisites

- Docker daemon running and accessible
- Docker CLI available in PATH
- Sufficient permissions to build and run containers

## Test Images

All custom Docker images built during integration tests use a unified naming scheme:
- **Image name**: `testcontainers_integration_test_container`
- **Image tag**: Short test case name (e.g., `dockerfile_string`, `single_file`, `multi_stage`, etc.)

## Automatic Cleanup

The integration test suite includes a **custom main function** that automatically cleans up test images after all tests complete:

```cpp
// tests/integration/main.cpp
int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  int test_result = RUN_ALL_TESTS();
  
  // Automatic cleanup of test images
  DockerCli::remove_images_by_name("testcontainers_integration_test_container");
  
  return test_result;
}
```

**Features:**
- ✅ Runs after all tests complete
- ✅ Removes all images with matching name
- ✅ Works on Windows, Linux, and macOS
- ✅ Provides informative output
- ✅ Handles cases where Docker is unavailable

## Running Tests

### Build and Run
```bash
# Build the project
cmake --build build/native --config Debug

# Run integration tests (with automatic cleanup)
ctest --test-dir build/native -C Debug -R IntegrationTests -V
```

### Run Directly
```bash
# Run the integration test executable directly
./build/native/tests/integration/Debug/testcontainers_integration_tests
```

Expected output:
```
Starting testcontainers-cxx integration tests...

[==========] Running 20 tests from 1 test suite.
...
[==========] 20 tests from 1 test suite ran.

========================================
Cleaning up test Docker images...
========================================
Docker is available. Removing images: testcontainers_integration_test_container
✓ Successfully cleaned up test images.
========================================
```

## Manual Cleanup

If you need to manually clean up test images:

### Windows PowerShell
```powershell
docker images "testcontainers_integration_test_container" --format "{{.Repository}}:{{.Tag}}" | ForEach-Object { docker image rm -f $_ }
```

### Windows CMD
```cmd
for /f "tokens=*" %i in ('docker images "testcontainers_integration_test_container" --format "{{.Repository}}:{{.Tag}}"') do docker image rm -f %i
```

### Linux/macOS
```bash
docker image rm -f $(docker images "testcontainers_integration_test_container" -q)
```

## Test Structure

### GenericBuildableImageIntegrationTest.cpp
Tests for building custom Docker images:

1. **Basic Build Tests** (2 tests)
   - BuildWithDockerfileString
   - BuildWithDockerfileFile

2. **Build with Files Tests** (2 tests)
   - BuildWithSingleFile
   - BuildWithMultipleFiles

3. **Build with Data Tests** (2 tests)
   - BuildWithBinaryData
   - BuildWithMultipleDataFiles

4. **Complex Build Scenarios** (4 tests)
   - BuildWithMultiStageDockerfile
   - BuildWithEnvironmentVariables
   - BuildWithWorkdir
   - BuildWithRunCommands

5. **File Permissions Tests** (2 tests)
   - BuildWithFilePermissions
   - BuildWithDirectoryCreation

6. **Package Installation Tests** (1 test)
   - BuildWithPackageInstallation

7. **Error Handling Tests** (2 tests)
   - BuildWithInvalidDockerfile
   - BuildWithNonExistentBaseImage

8. **Edge Cases Tests** (3 tests)
   - BuildMinimalImage
   - BuildWithLongRunningCommand
   - BuildWithUnicodeContent

9. **Rebuild and Caching Tests** (1 test)
   - RebuildSameImage

### RedisTest.cpp
Tests using official Redis image from Docker Hub.

## Troubleshooting

### Tests fail with "Docker daemon not found"
Ensure Docker is running and the `docker` command is in your PATH:
```bash
docker version
```

### Tests fail with "permission denied"
On Linux, you may need to add your user to the `docker` group:
```bash
sudo usermod -aG docker $USER
# Log out and log back in
```

### Cleanup fails
If automatic cleanup fails, you can manually remove images:
```bash
docker image rm -f $(docker images "testcontainers_integration_test_container" -q)
```

### Images still present after tests
Check for running containers that may prevent image removal:
```bash
docker ps -a --filter ancestor=testcontainers_integration_test_container
docker rm -f $(docker ps -a -q --filter ancestor=testcontainers_integration_test_container)
```

