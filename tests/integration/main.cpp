#include <gtest/gtest.h>

#include <iostream>

#include "testutils/DockerCli.hpp"

// Name of test images created during integration tests
constexpr const char* TEST_IMAGE_NAME = "testcontainers_integration_test_container";

/**
 * @brief Custom main function for integration tests
 * 
 * Workflow:
 * 1. Checks Docker availability before running tests
 * 2. Executes all GTest test cases
 * 3. Cleans up Docker images created during tests
 * 4. Reports cleanup status
 */
int main(int argc, char** argv) {
  std::cout << "========================================" << std::endl;
  std::cout << "Testcontainers-CXX Integration Tests" << std::endl;
  std::cout << "========================================" << std::endl;

  // Check Docker availability before running tests
  std::cout << "Checking Docker availability..." << std::endl;
  bool docker_available = testcontainers::test_utils::DockerCli::is_docker_available();

  if (docker_available) {
    std::cout << "✓ Docker is available and running." << std::endl;
  } else {
    std::cerr << "✗ ERROR: Docker is not available!" << std::endl;
    std::cerr << "  Integration tests require a running Docker daemon." << std::endl;
    std::cerr << "  Please start Docker and try again." << std::endl;
    return 1;
  }

  std::cout << "========================================\n" << std::endl;

  // Initialize and run all tests
  ::testing::InitGoogleTest(&argc, argv);

  int test_result = RUN_ALL_TESTS();

  // Cleanup: Remove all test images
  std::cout << "\n========================================" << std::endl;
  std::cout << "Cleaning up test Docker images..." << std::endl;
  std::cout << "========================================" << std::endl;
  std::cout << "Removing images: " << TEST_IMAGE_NAME << std::endl;

  bool cleanup_success =
      testcontainers::test_utils::DockerCli::remove_images_by_name(TEST_IMAGE_NAME);

  if (cleanup_success) {
    std::cout << "✓ Successfully cleaned up test images." << std::endl;
  } else {
    std::cout << "⚠ Warning: Failed to clean up some test images." << std::endl;
    std::cout << "  This may happen if containers are still running." << std::endl;
    std::cout << "  You may need to manually remove them using:" << std::endl;
    std::cout << "  docker image rm -f $(docker images \"" << TEST_IMAGE_NAME << "\" -q)"
              << std::endl;
  }

  std::cout << "========================================\n" << std::endl;

  return test_result;
}

