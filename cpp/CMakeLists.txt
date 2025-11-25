# C++ testcontainers library

add_library(testcontainers) # shared or static, depending on BUILD_SHARED_LIBS

add_library(testcontainers::testcontainers ALIAS testcontainers)

target_sources(testcontainers
	PUBLIC
    lib/testcontainers/interfaces/IContainer.hpp
    lib/testcontainers/interfaces/ISyncRunner.hpp
    lib/testcontainers/interfaces/IImage.hpp
    lib/testcontainers/interfaces/IImageExt.hpp
    lib/testcontainers/interfaces/IRustObject.hpp

    lib/testcontainers/core/wait/ExitWaitStrategy.hpp
    lib/testcontainers/core/wait/HealthWaitStrategy.hpp
    lib/testcontainers/core/wait/LogWaitStrategy.hpp
    lib/testcontainers/core/wait/WaitFor.hpp
    lib/testcontainers/core/CgroupnsMode.hpp
    lib/testcontainers/core/ContainerPort.hpp
    lib/testcontainers/core/CopyDataSource.hpp
    lib/testcontainers/core/ExecCommand.hpp
    lib/testcontainers/core/Healthcheck.hpp
    lib/testcontainers/core/Host.hpp
    lib/testcontainers/core/Mount.hpp
    lib/testcontainers/core/SyncExecResult.hpp

    lib/testcontainers/system/Path.hpp
    lib/testcontainers/system/ip/IpAddr.hpp
    lib/testcontainers/system/ip/Ipv4Addr.hpp
    lib/testcontainers/system/ip/Ipv6Addr.hpp
    lib/testcontainers/system/UrlHost.hpp

    lib/testcontainers/Container.hpp
    lib/testcontainers/Error.hpp
    lib/testcontainers/ContainerRequest.hpp
    lib/testcontainers/GenericBuildableImage.hpp
    lib/testcontainers/GenericImage.hpp
    lib/testcontainers/testcontainers.hpp
    lib/testcontainers/Version.hpp

    PRIVATE
    lib/testcontainers/core/wait/ExitWaitStrategy.cpp
    lib/testcontainers/core/wait/HealthWaitStrategy.cpp
    lib/testcontainers/core/wait/LogWaitStrategy.cpp
    lib/testcontainers/core/wait/WaitFor.cpp
    lib/testcontainers/core/CgroupnsMode.cpp
    lib/testcontainers/core/ContainerPort.cpp
    lib/testcontainers/core/CopyDataSource.cpp
    lib/testcontainers/core/ExecCommand.cpp
    lib/testcontainers/core/Healthcheck.cpp
    lib/testcontainers/core/Host.cpp
    lib/testcontainers/core/Mount.cpp
    lib/testcontainers/core/SyncExecResult.cpp

    lib/testcontainers/system/ip/IpAddr.cpp
    lib/testcontainers/system/ip/Ipv4Addr.cpp
    lib/testcontainers/system/ip/Ipv6Addr.cpp
    lib/testcontainers/system/Path.cpp
    lib/testcontainers/system/UrlHost.cpp

    lib/testcontainers/Container.cpp
    lib/testcontainers/Error.cpp
    lib/testcontainers/ContainerRequest.cpp
    lib/testcontainers/GenericBuildableImage.cpp
    lib/testcontainers/GenericImage.cpp
    lib/testcontainers/Version.cpp

    util/details/OptionHelper.hpp
    util/details/VectorHelper.hpp
    util/details/BoxHelper.hpp
    util/details/ErrorHelper.hpp
)

target_link_libraries(testcontainers
    PRIVATE
    rust_tc_bridge
)

target_include_directories(testcontainers
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/lib>
    $<INSTALL_INTERFACE:include>

    PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/util>
)

set_target_properties(testcontainers PROPERTIES
    WINDOWS_EXPORT_ALL_SYMBOLS ON
)

