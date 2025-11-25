cmake_minimum_required(VERSION 3.15)
project(PackageTest CXX)

find_package(testcontainers-cxx CONFIG REQUIRED)

message(STATUS "BUILD_TESTING = ${BUILD_TESTING}")

add_executable(example src/example.cpp)
target_link_libraries(example testcontainers::testcontainers)
