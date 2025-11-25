#include <gmock/gmock.h>
#include <gtest/gtest.h>


#include <chrono>
#include <thread>

#include <testcontainers/testcontainers.hpp>

using namespace testcontainers;
using ::testing::HasSubstr;

// ============================================================================
// Container Lifecycle Tests
// ============================================================================

TEST(ContainerIntegrationTest, BasicContainerStart) {
  auto container = GenericImage("alpine", "latest").with_cmd({"sh", "-c", "sleep 200"}).start();
  EXPECT_TRUE(container.is_valid());
  EXPECT_TRUE(container.is_running());
}

TEST(ContainerIntegrationTest, ContainerStopAndCheck) {
  auto container = GenericImage("alpine", "latest").with_cmd({"sh", "-c", "sleep 200"}).start();
  EXPECT_TRUE(container.is_running());

  container.stop();
  EXPECT_FALSE(container.is_running());
}

TEST(ContainerIntegrationTest, ContainerStopWithTimeout) {
  auto container = GenericImage("alpine", "latest").with_cmd({"sh", "-c", "sleep 200"}).start();
  EXPECT_TRUE(container.is_running());

  container.stop_with_timeout(2);
  EXPECT_FALSE(container.is_running());
}

TEST(ContainerIntegrationTest, ContainerStopStartCycle) {
  auto container = GenericImage("alpine", "latest").with_cmd({"sh", "-c", "sleep 200"}).start();
  EXPECT_TRUE(container.is_running());

  container.stop();
  EXPECT_FALSE(container.is_running());

  container.start();
  EXPECT_TRUE(container.is_running());
}

TEST(ContainerIntegrationTest, ContainerMultipleStopStartCycles) {
  auto container = GenericImage("alpine", "latest").with_cmd({"sh", "-c", "sleep 200"}).start();

  for (int i = 0; i < 3; ++i) {
    EXPECT_TRUE(container.is_running());

    container.stop();
    EXPECT_FALSE(container.is_running());

    container.start();
  }

  EXPECT_TRUE(container.is_running());
}

// ============================================================================
// Container Exit Code Tests
// ============================================================================

TEST(ContainerIntegrationTest, ContainerExitCodeSuccess) {
  auto container = GenericImage("alpine", "latest").with_cmd({"sh", "-c", "exit 0"}).start();

  // Wait for container to finish
  container.stdout_to_string();

  EXPECT_EQ(container.exit_code(), std::optional<std::int64_t>(0));
}

TEST(ContainerIntegrationTest, ContainerExitCodeFailure) {
  auto container = GenericImage("alpine", "latest").with_cmd({"sh", "-c", "exit 42"}).start();

  container.stdout_to_string();

  EXPECT_EQ(container.exit_code(), std::optional<std::int64_t>(42));
}

TEST(ContainerIntegrationTest, ContainerExitCodeRunning) {
  auto container = GenericImage("alpine", "latest").with_cmd({"sh", "-c", "sleep 200"}).start();

  // Container is still running, no exit code yet
  EXPECT_EQ(container.exit_code(), std::nullopt);
}

// ============================================================================
// Container Output Tests
// ============================================================================

TEST(ContainerIntegrationTest, ContainerStdout) {
  std::string expected_output = "Hello from container stdout";

  auto container = GenericImage("alpine", "latest")
                       .with_cmd({"sh", "-c", "echo '" + expected_output + "'"})
                       .start();

  ASSERT_THAT(container.stdout_to_string(), HasSubstr(expected_output));
}

TEST(ContainerIntegrationTest, ContainerStderr) {
  std::string expected_error = "Error from container";

  auto container = GenericImage("alpine", "latest")
                       .with_cmd({"sh", "-c", "echo '" + expected_error + "' >&2"})
                       .start();

  ASSERT_THAT(container.stderr_to_string(), HasSubstr(expected_error));
}

TEST(ContainerIntegrationTest, ContainerStdoutAndStderr) {
  std::string stdout_msg = "Stdout message";
  std::string stderr_msg = "Stderr message";

  auto container
      = GenericImage("alpine", "latest")
            .with_cmd({"sh", "-c", "echo '" + stdout_msg + "' && echo '" + stderr_msg + "' >&2"})
            .start();

  ASSERT_THAT(container.stdout_to_string(), HasSubstr(stdout_msg));
  ASSERT_THAT(container.stderr_to_string(), HasSubstr(stderr_msg));
}

TEST(ContainerIntegrationTest, ContainerMultilineStdout) {
  auto container = GenericImage("alpine", "latest")
                       .with_cmd({"sh", "-c", "echo 'Line 1' && echo 'Line 2' && echo 'Line 3'"})
                       .start();

  auto stdout_str = container.stdout_to_string();
  ASSERT_THAT(stdout_str, HasSubstr("Line 1"));
  ASSERT_THAT(stdout_str, HasSubstr("Line 2"));
  ASSERT_THAT(stdout_str, HasSubstr("Line 3"));
}

// ============================================================================
// Container Port Mapping Tests
// ============================================================================

TEST(ContainerIntegrationTest, ContainerGetHostPortIpv4) {
  auto container = GenericImage("alpine", "latest")
                       .with_exposed_port(ContainerPort::Tcp(8080))
                       .with_cmd({"sh", "-c", "sleep 200"})
                       .start();
  EXPECT_TRUE(container.is_running());

  auto host_port = container.get_host_port_ipv4(ContainerPort::Tcp(8080));
  EXPECT_GT(host_port, 0);
}

TEST(ContainerIntegrationTest, ContainerGetHostPortMultiplePorts) {
  auto container = GenericImage("alpine", "latest")
                       .with_exposed_port(ContainerPort::Tcp(80))
                       .with_exposed_port(ContainerPort::Tcp(443))
                       .with_exposed_port(ContainerPort::Tcp(8080))
                       .with_cmd({"sh", "-c", "sleep 200"})
                       .start();

  EXPECT_TRUE(container.is_running());

  auto port_80 = container.get_host_port_ipv4(ContainerPort::Tcp(80));
  auto port_443 = container.get_host_port_ipv4(ContainerPort::Tcp(443));
  auto port_8080 = container.get_host_port_ipv4(ContainerPort::Tcp(8080));

  EXPECT_GT(port_80, 0);
  EXPECT_GT(port_443, 0);
  EXPECT_GT(port_8080, 0);

  // All ports should be different
  EXPECT_NE(port_80, port_443);
  EXPECT_NE(port_80, port_8080);
  EXPECT_NE(port_443, port_8080);
}

TEST(ContainerIntegrationTest, ContainerGetHostPortUdp) {
  auto container = GenericImage("alpine", "latest")
                       .with_exposed_port(ContainerPort::Udp(53))
                       .with_cmd({"sh", "-c", "sleep 200"})
                       .start();

  EXPECT_TRUE(container.is_running());

  auto host_port = container.get_host_port_ipv4(ContainerPort::Udp(53));
  EXPECT_GT(host_port, 0);
}

// ============================================================================
// Container Host Tests
// ============================================================================

TEST(ContainerIntegrationTest, ContainerGetHost) {
  auto container = GenericImage("alpine", "latest").with_cmd({"sh", "-c", "sleep 200"}).start();

  EXPECT_TRUE(container.is_running());

  auto host = container.get_host();
  EXPECT_TRUE(host.is_domain() || host.is_ipv4() || host.is_ipv6());
}
// ============================================================================
// Container Exec Tests
// ============================================================================

TEST(ContainerIntegrationTest, ContainerExecSimpleCommand) {
  auto container = GenericImage("alpine", "latest").with_cmd({"sh", "-c", "sleep 200"}).start();
  EXPECT_TRUE(container.is_running());

  auto result = container.exec(ExecCommand({"echo", "Hello from exec"}));
  ASSERT_THAT(result.stdout_to_string(), HasSubstr("Hello from exec"));
  EXPECT_EQ(result.exit_code(), std::optional<std::int64_t>(0));
}

TEST(ContainerIntegrationTest, ContainerExecWithNonZeroExitCode) {
  auto container = GenericImage("alpine", "latest").with_cmd({"sh", "-c", "sleep 200"}).start();
  EXPECT_TRUE(container.is_running());

  auto result = container.exec(ExecCommand({"sh", "-c", "exit 5"}));

  result.stdout_to_string(); // wait for the command to finish

  EXPECT_EQ(result.exit_code(), std::optional<std::int64_t>(5));
}

TEST(ContainerIntegrationTest, ContainerExecListFiles) {
  auto container = GenericImage("alpine", "latest").with_cmd({"sh", "-c", "sleep 200"}).start();
  EXPECT_TRUE(container.is_running());

  auto result = container.exec(ExecCommand({"ls", "/etc"}));
  EXPECT_FALSE(result.stdout_to_string().empty());
  EXPECT_EQ(result.exit_code(), std::optional<std::int64_t>(0));
}

TEST(ContainerIntegrationTest, ContainerExecWithStderr) {
  auto container = GenericImage("alpine", "latest").with_cmd({"sh", "-c", "sleep 200"}).start();
  EXPECT_TRUE(container.is_running());

  auto result = container.exec(ExecCommand({"sh", "-c", "echo 'error' >&2"}));
  ASSERT_THAT(result.stderr_to_string(), HasSubstr("error"));
}

TEST(ContainerIntegrationTest, ContainerExecMultipleCommands) {
  auto container = GenericImage("alpine", "latest").with_cmd({"sh", "-c", "sleep 200"}).start();
  EXPECT_TRUE(container.is_running());

  // Execute first command
  auto result1 = container.exec(ExecCommand({"echo", "Command 1"}));
  ASSERT_THAT(result1.stdout_to_string(), HasSubstr("Command 1"));

  // Execute second command
  auto result2 = container.exec(ExecCommand({"echo", "Command 2"}));
  ASSERT_THAT(result2.stdout_to_string(), HasSubstr("Command 2"));
}

// ============================================================================
// Container Remove Tests
// ============================================================================

TEST(ContainerIntegrationTest, ContainerRemoveAfterStop) {
  auto container = GenericImage("alpine", "latest").with_cmd({"sh", "-c", "sleep 200"}).start();

  EXPECT_TRUE(container.is_running());
  EXPECT_TRUE(container.is_valid());

  container.stop();

  // Remove the container
  Container::rm(std::move(container));

  // After rm, the container should not be valid
  EXPECT_FALSE(container.is_valid());
}

TEST(ContainerIntegrationTest, ContainerRemoveAfterExit) {
  auto container = GenericImage("alpine", "latest").with_cmd({"sh", "-c", "exit 0"}).start();

  // Wait for container to exit
  container.stdout_to_string();

  EXPECT_FALSE(container.is_running());

  // Remove the container
  Container::rm(std::move(container));

  EXPECT_FALSE(container.is_valid());
}

// ============================================================================
// Complex Scenarios
// ============================================================================

TEST(ContainerIntegrationTest, ComplexWorkflowWithExec) {
  auto container = GenericImage("alpine", "latest").with_cmd({"sh", "-c", "sleep 200"}).start();
  EXPECT_TRUE(container.is_running());

  // Create a file via exec
  auto create_result
      = container.exec(ExecCommand({"sh", "-c", "echo 'test content' > /tmp/test.txt"}));
  create_result.stdout_to_string(); // wait for the command to finish
  EXPECT_EQ(create_result.exit_code(), std::optional<std::int64_t>(0));

  // Read the file via exec
  auto read_result = container.exec(ExecCommand({"cat", "/tmp/test.txt"}));
  ASSERT_THAT(read_result.stdout_to_string(), HasSubstr("test content"));

  // Stop the container
  container.stop();
  EXPECT_FALSE(container.is_running());
}

TEST(ContainerIntegrationTest, ContainerWithExposedPortsAndExec) {
  auto container = GenericImage("alpine", "latest")
                       .with_exposed_port(ContainerPort::Tcp(8080))
                       .with_exposed_port(ContainerPort::Tcp(9090))
                       .with_cmd({"sh", "-c", "sleep 200"})
                       .start();

  EXPECT_TRUE(container.is_running());

  // Get host ports
  auto port_8080 = container.get_host_port_ipv4(ContainerPort::Tcp(8080));
  auto port_9090 = container.get_host_port_ipv4(ContainerPort::Tcp(9090));

  EXPECT_GT(port_8080, 0);
  EXPECT_GT(port_9090, 0);

  // Execute command inside container
  auto result = container.exec(ExecCommand({"echo", "Ports are mapped"}));

  ASSERT_THAT(result.stdout_to_string(), HasSubstr("Ports are mapped"));
}

TEST(ContainerIntegrationTest, LongRunningContainerWithPeriodicChecks) {
  auto container = GenericImage("alpine", "latest").with_cmd({"sh", "-c", "sleep 200"}).start();
  EXPECT_TRUE(container.is_running());

  // Check status periodically
  for (int i = 0; i < 5; ++i) {
    std::this_thread::sleep_for(std::chrono::seconds(1)); // wait for 1 second
    EXPECT_TRUE(container.is_running());

    // Execute a command to verify container is responsive
    auto result = container.exec(ExecCommand({"echo", "alive"}));
    ASSERT_THAT(result.stdout_to_string(), HasSubstr("alive"));
  }
}

TEST(ContainerIntegrationTest, ContainerStdoutStderrAfterStop) {
  std::string stdout_msg = "Stdout before stop";
  std::string stderr_msg = "Stderr before stop";

  auto container
      = GenericImage("alpine", "latest")
            .with_cmd({"sh", "-c",
                       "echo '" + stdout_msg + "' && echo '" + stderr_msg + "' >&2 && sleep 200"})
            .start();

  container.stop();

  // Should still be able to read stdout/stderr after stop
  ASSERT_THAT(container.stdout_to_string(), HasSubstr(stdout_msg));
  ASSERT_THAT(container.stderr_to_string(), HasSubstr(stderr_msg));
}

TEST(ContainerIntegrationTest, ContainerWithEnvVarsAndExec) {
  auto container = GenericImage("alpine", "latest")
                       .with_env_var("TEST_VAR", "test_value")
                       .with_env_var("ANOTHER_VAR", "another_value")
                       .with_cmd({"sh", "-c", "sleep 200"})
                       .start();

  EXPECT_TRUE(container.is_running());

  // Check environment variables via exec
  auto result = container.exec(ExecCommand({"env"}));

  auto stdout_str = result.stdout_to_string();
  ASSERT_THAT(stdout_str, HasSubstr("TEST_VAR"));
  ASSERT_THAT(stdout_str, HasSubstr("ANOTHER_VAR"));
}
