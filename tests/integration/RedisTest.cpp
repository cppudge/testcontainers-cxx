#include <gtest/gtest.h>

#include <testcontainers/testcontainers.hpp>

using namespace testcontainers;

TEST(RedisTest, StartAndGetMappedPort) {

  auto container = GenericImage("redis", "7.2")
                       .with_exposed_port(ContainerPort::Tcp(6379))
                       .with_wait_for(WaitFor::message_on_stdout("Ready to accept connections"))
                       .start();

  auto host_port = container.get_host_port_ipv4(ContainerPort::Tcp(6379));

  EXPECT_GT(host_port, 0);
  std::cout << "Redis mapped port: " << host_port << std::endl;

  // Container will be auto-cleaned when it goes out of scope
}

// TEST(RedisTest, MultipleExposedPorts) {
//     // Start container with multiple exposed ports
//     auto container = GenericImage("redis", "7.2")
//         .with_exposed_port(6379)
//         .with_exposed_port(6380)
//         .with_wait_for_message("Ready to accept connections")
//         .start();

//     ASSERT_TRUE(container.is_valid());

//     auto port1 = container.mapped_port(6379);
//     auto port2 = container.mapped_port(6380);

//     EXPECT_GT(port1, 0);
//     EXPECT_GT(port2, 0);
//     EXPECT_NE(port1, port2); // Ports should be different

//     std::cout << "Port 6379 -> " << port1 << std::endl;
//     std::cout << "Port 6380 -> " << port2 << std::endl;
// }

// TEST(RedisTest, WithEnvironmentVariable) {
//     // Start Redis with custom configuration via env var
//     auto container = GenericImage("redis", "7.2")
//         .with_exposed_port(6379)
//         .with_env("REDIS_ARGS", "--appendonly yes")
//         .with_wait_for_message("Ready to accept connections")
//         .start();

//     ASSERT_TRUE(container.is_valid());

//     auto port = container.mapped_port(6379);
//     EXPECT_GT(port, 0);
// }

// TEST(RedisTest, RAIIAutoCleanup) {
//     uint16_t mapped_port;

//     {
//         auto container = GenericImage("redis", "7.2")
//             .with_exposed_port(6379)
//             .with_wait_for_message("Ready to accept connections")
//             .start();

//         mapped_port = container.mapped_port(6379);
//         EXPECT_GT(mapped_port, 0);

//         // Container goes out of scope here and will be auto-cleaned by
//         Docker
//     }

//     // If we reach here, RAII cleanup worked without crashing
//     SUCCEED();
// }

// TEST(RedisTest, MoveSemantics) {
//     auto container1 = GenericImage("redis", "7.2")
//         .with_exposed_port(6379)
//         .with_wait_for_message("Ready to accept connections")
//         .start();

//     ASSERT_TRUE(container1.is_valid());
//     auto port1 = container1.mapped_port(6379);

//     // Move to container2
//     Container container2 = std::move(container1);

//     ASSERT_TRUE(container2.is_valid());
//     auto port2 = container2.mapped_port(6379);

//     EXPECT_EQ(port1, port2); // Should be the same container
// }
