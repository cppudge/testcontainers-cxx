include(FetchContent)

if(CMAKE_CROSSCOMPILING)
    if (NOT DEFINED Rust_CARGO_TARGET)
        message(FATAL_ERROR "Rust_CARGO_TARGET must be set manually when crosscompiling")
    endif()
endif()

FetchContent_Declare(
    Corrosion
    GIT_REPOSITORY https://github.com/corrosion-rs/corrosion.git
    GIT_TAG v0.5.2
)
FetchContent_MakeAvailable(Corrosion)

corrosion_import_crate(MANIFEST_PATH Cargo.toml)

corrosion_add_cxxbridge(rust_tc_bridge CRATE tc_bridge FILES lib.rs)

if (DEFINED CMAKE_POSITION_INDEPENDENT_CODE)
    set(RUST_PIC_FLAG_ENV_VAR "RUST_PIC=$<IF:$<BOOL:${CMAKE_POSITION_INDEPENDENT_CODE}>,1,0>")
endif()

cmake_language(GET_MESSAGE_LOG_LEVEL _cmake_log_level)
if(_cmake_log_level STREQUAL "VERBOSE" OR _cmake_log_level STREQUAL "DEBUG" OR _cmake_log_level STREQUAL "TRACE")
    corrosion_set_cargo_flags(tc_bridge -vv)
endif()

corrosion_set_env_vars(tc_bridge
    "CC=${CMAKE_C_COMPILER}"
    "CXX=${CMAKE_CXX_COMPILER}"
    "CXXFLAGS=${CMAKE_CXX_FLAGS}"
    RUST_CXX_STD=$<IF:$<BOOL:${CMAKE_CXX_EXTENSIONS}>,gnu++,c++>${CMAKE_CXX_STANDARD}
    ${RUST_PIC_FLAG_ENV_VAR}
)
