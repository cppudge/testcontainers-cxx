# Integration tests (require Docker)
add_executable(testcontainers_integration_tests 
    main.cpp
    RedisTest.cpp
    GenericBuildableImageIntegrationTest.cpp
    GenericImageIntegrationTest.cpp
    ContainerRequestIntegrationTest.cpp
    ContainerIntegrationTest.cpp
)

target_link_libraries(testcontainers_integration_tests 
    testcontainers
    testcontainers_test_utils
    GTest::gtest
    # Note: Using custom main.cpp instead of GTest::gtest_main for cleanup
)

add_test(NAME IntegrationTests COMMAND testcontainers_integration_tests)
