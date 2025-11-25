# ===== Conan integration =====
if (DEFINED SKIP_CONAN_PROVIDER_CMAKE)
    message(STATUS "Skipping conan provider cmake because SKIP_CONAN_PROVIDER_CMAKE is defined")
else()
    message(STATUS "Using conan provider cmake for automatic conan installation")

    if(NOT DEFINED ENV{CONAN_HOME})
        file(MAKE_DIRECTORY "${CMAKE_SOURCE_DIR}/.conan2")
        set(ENV{CONAN_HOME} "${CMAKE_SOURCE_DIR}/.conan2")
    endif()

    set(CONAN_HOST_PROFILE "auto-cmake" CACHE STRING "Conan host profile")
    set(CONAN_BUILD_PROFILE "auto-cmake" CACHE STRING "Conan build profile")
    set(CMAKE_PROJECT_TOP_LEVEL_INCLUDES "${CMAKE_SOURCE_DIR}/cmake/cmake-conan/conan_provider.cmake")
endif()

# ===== Project configuration =====
cmake_minimum_required(VERSION 3.20)
project(testcontainers LANGUAGES CXX)

if(NOT DEFINED CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 20 CACHE STRING "C++ standard (e.g. 17, 20, 23)")
    set(CMAKE_CXX_STANDARD_REQUIRED ON CACHE STRING "")
    set(CMAKE_CXX_EXTENSIONS OFF CACHE STRING "")
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin CACHE PATH "Output directory for executables")

if(NOT DEFINED BUILD_SHARED_LIBS)
    set(BUILD_SHARED_LIBS ON CACHE BOOL "Build shared libraries")
endif()

if (BUILD_SHARED_LIBS)
    set(CMAKE_POSITION_INDEPENDENT_CODE ON)
endif()

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDLL") # for Windows integration with rust

message(STATUS "SKIP_CONAN_PROVIDER_CMAKE = ${SKIP_CONAN_PROVIDER_CMAKE}")
message(STATUS "CMAKE_CXX_STANDARD = ${CMAKE_CXX_STANDARD}")
message(STATUS "CMAKE_CXX_STANDARD_REQUIRED = ${CMAKE_CXX_STANDARD_REQUIRED}")
message(STATUS "CMAKE_CXX_EXTENSIONS = ${CMAKE_CXX_EXTENSIONS}")
message(STATUS "CMAKE_C_COMPILER = ${CMAKE_C_COMPILER}")
message(STATUS "CMAKE_CXX_COMPILER = ${CMAKE_CXX_COMPILER}")
message(STATUS "CMAKE_CXX_FLAGS = ${CMAKE_CXX_FLAGS}")
message(STATUS "CMAKE_CXX_COMPILER_WORKS = ${CMAKE_CXX_COMPILER_WORKS}")
message(STATUS "CMAKE_CXX_COMPILER_FORCED = ${CMAKE_CXX_COMPILER_FORCED}")
message(STATUS "CMAKE_CROSSCOMPILING = ${CMAKE_CROSSCOMPILING}")
message(STATUS "Rust_CARGO_TARGET = ${Rust_CARGO_TARGET}")
message(STATUS "CMAKE_HOST_SYSTEM_PROCESSOR = ${CMAKE_HOST_SYSTEM_PROCESSOR}")
message(STATUS "CMAKE_SYSTEM_PROCESSOR = ${CMAKE_SYSTEM_PROCESSOR}")
message(STATUS "CMAKE_RUNTIME_OUTPUT_DIRECTORY = ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
message(STATUS "BUILD_SHARED_LIBS = ${BUILD_SHARED_LIBS}")
message(STATUS "BUILD_TESTING = ${BUILD_TESTING}")
message(STATUS "CMAKE_POSITION_INDEPENDENT_CODE = ${CMAKE_POSITION_INDEPENDENT_CODE}")
message(STATUS "CMAKE_MSVC_RUNTIME_LIBRARY = ${CMAKE_MSVC_RUNTIME_LIBRARY}")

# ===== C++ dependencies =====
if (NOT BUILD_TESTING STREQUAL OFF)
    find_package(GTest CONFIG REQUIRED)
endif()

# ===== Rust-side of the bridge =====
add_subdirectory(rust)

# ===== C++-side of the bridge =====
add_subdirectory(cpp)

# ===== Tests =====
if (NOT BUILD_TESTING STREQUAL OFF)
    enable_testing()
    add_subdirectory(tests)
endif()

include(CheckPIESupported)
check_pie_supported()

# ===== Installation =====
include(GNUInstallDirs)

if(NOT BUILD_SHARED_LIBS)
    install(FILES $<TARGET_FILE:tc_bridge-static>
        DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )

    install(TARGETS rust_tc_bridge
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )
endif()

install(TARGETS testcontainers
    EXPORT testcontainersTargets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cpp/lib/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    FILES_MATCHING
    PATTERN "*.hpp"
)
