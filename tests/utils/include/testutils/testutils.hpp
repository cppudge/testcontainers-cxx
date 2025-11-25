#pragma once

/**
 * @file testutils.hpp
 * @brief Convenience header to include all test utilities
 * 
 * Include this header to get access to all testcontainers test utilities:
 * - TempFile: RAII temporary file management
 * - DockerCli: Docker CLI helper functions
 */

#include "testutils/TempFile.hpp"
#include "testutils/DockerCli.hpp"

