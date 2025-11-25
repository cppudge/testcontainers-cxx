add_executable(testcontainers_unit_tests 
    VersionTest.cpp 
    WaitForTest.cpp
    GenericBuildableImageTest.cpp
    GenericImageTest.cpp
    ContainerRequestTest.cpp
    IpAddrTest.cpp
    Ipv4AddrTest.cpp
    Ipv6AddrTest.cpp
    PathTest.cpp
    UrlHostTest.cpp
    MountTest.cpp
    ContainerPortTest.cpp
    HostTest.cpp
    CopyDataSourceTest.cpp
    CgroupnsModeTest.cpp
    HealthcheckTest.cpp
)

target_link_libraries(testcontainers_unit_tests 
    testcontainers
    testcontainers_test_utils
    GTest::gtest 
    GTest::gtest_main
)

add_test(NAME UnitTests COMMAND testcontainers_unit_tests)
