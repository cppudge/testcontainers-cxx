# Testcontainers Test Utilities Library
# Shared utilities for both unit and integration tests

add_library(testcontainers_test_utils STATIC
    include/testutils/TempFile.hpp
    src/TempFile.cpp
    include/testutils/DockerCli.hpp
    src/DockerCli.cpp
)

target_include_directories(testcontainers_test_utils
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# No external dependencies needed for now
# Just C++ standard library

