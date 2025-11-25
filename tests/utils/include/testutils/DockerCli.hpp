#pragma once

#include <string>

namespace testcontainers {
namespace test_utils {

/**
 * @brief Docker CLI utility functions
 * 
 * Provides helper functions to interact with Docker daemon via CLI commands.
 */
class DockerCli {
public:
  /**
   * @brief Remove all Docker images matching the given name
   * @param image_name Name of the image(s) to remove
   * @return true if command executed successfully, false otherwise
   * 
   * This function attempts to remove all images with the specified name,
   * regardless of their tags. It suppresses error output to avoid noise
   * when images are in use or don't exist.
   */
  static bool remove_images_by_name(const std::string &image_name);

  /**
   * @brief Remove a specific Docker image by name and tag
   * @param name Image name
   * @param tag Image tag
   * @return true if command executed successfully, false otherwise
   */
  static bool remove_image(const std::string &name, const std::string &tag);

  /**
   * @brief Check if Docker daemon is running
   * @return true if Docker is available and running, false otherwise
   */
  static bool is_docker_available();
};

} // namespace test_utils
} // namespace testcontainers

