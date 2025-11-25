#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <testcontainers/testcontainers.hpp>

#include "testutils/TempFile.hpp"

using namespace testcontainers;
using namespace testcontainers::test_utils;

using ::testing::HasSubstr;

// ============================================================================
// Basic Container Operations
// ============================================================================

TEST(GenericImageIntegrationTest, BasicStart) {
  auto container = GenericImage("alpine", "latest").with_cmd({"sh", "-c", "sleep 20"}).start();

  EXPECT_TRUE(container.is_running());
}

TEST(GenericImageIntegrationTest, StartWithEntrypoint) {
  auto container = GenericImage("alpine", "latest")
                       .with_entrypoint("/bin/sh")
                       .with_cmd({"-c", "echo 'test'"})
                       .start();

  EXPECT_THAT(container.stdout_to_string(), HasSubstr("test"));
}

TEST(GenericImageIntegrationTest, StartWithWaitForLogMessage) {
  auto container = GenericImage("alpine", "latest")
                       .with_wait_for(WaitFor::message_on_stdout("Ready to accept connections"))
                       .with_cmd({"sh", "-c", "echo 'Ready to accept connections' && sleep 100"})
                       .start();

  EXPECT_TRUE(container.is_running());
}

// ============================================================================
// Environment Variables
// ============================================================================

TEST(GenericImageIntegrationTest, StartWithEnvVar) {
  auto container = GenericImage("alpine", "latest")
                       .with_env_var("TEST_VAR", "test_value")
                       .with_cmd({"sh", "-c", "echo $TEST_VAR"})
                       .start();

  EXPECT_THAT(container.stdout_to_string(), HasSubstr("test_value"));
}

TEST(GenericImageIntegrationTest, StartWithMultipleEnvVars) {
  auto container = GenericImage("alpine", "latest")
                       .with_env_var("VAR1", "value1")
                       .with_env_var("VAR2", "value2")
                       .with_cmd({"sh", "-c", "echo $VAR1-$VAR2"})
                       .start();

  EXPECT_THAT(container.stdout_to_string(), HasSubstr("value1-value2"));
}

// ============================================================================
// Port Mapping
// ============================================================================

TEST(GenericImageIntegrationTest, StartWithExposedPort) {
  auto container = GenericImage("alpine", "latest")
                       .with_exposed_port(ContainerPort::Tcp(8080))
                       .with_cmd({"sh", "-c", "sleep 200"})
                       .start();

  EXPECT_TRUE(container.is_running());

  // Get the mapped host port
  auto host_port = container.get_host_port_ipv4(ContainerPort::Tcp(8080));
  EXPECT_GT(host_port, 0);
}

TEST(GenericImageIntegrationTest, StartWithMappedPort) {
  std::uint16_t host_port = 18080;

  auto container = GenericImage("alpine", "latest")
                       .with_mapped_port(host_port, ContainerPort::Tcp(80))
                       .with_cmd({"sh", "-c", "sleep 200"})
                       .start();

  EXPECT_TRUE(container.is_running());

  // Verify the port mapping
  auto actual_host_port = container.get_host_port_ipv4(ContainerPort::Tcp(80));
  EXPECT_EQ(actual_host_port, host_port);
}

// ============================================================================
// Container Naming and Metadata
// ============================================================================

TEST(GenericImageIntegrationTest, StartWithContainerName) {
  auto container = GenericImage("alpine", "latest")
                       .with_container_name("test_container_name")
                       .with_cmd({"sh", "-c", "sleep 200"})
                       .start();

  EXPECT_TRUE(container.is_running());
}

TEST(GenericImageIntegrationTest, StartWithLabels) {
  auto container = GenericImage("alpine", "latest")
                       .with_label("test.label.key1", "value1")
                       .with_label("test.label.key2", "value2")
                       .with_cmd({"sh", "-c", "sleep 200"})
                       .start();

  EXPECT_TRUE(container.is_running());
}

TEST(GenericImageIntegrationTest, StartWithLabelsVector) {
  std::vector<std::pair<std::string, std::string>> labels
      = {{"label1", "value1"}, {"label2", "value2"}, {"label3", "value3"}};

  auto container = GenericImage("alpine", "latest")
                       .with_labels(labels)
                       .with_cmd({"sh", "-c", "sleep 200"})
                       .start();

  EXPECT_TRUE(container.is_running());
}

TEST(GenericImageIntegrationTest, StartWithHostname) {
  auto container = GenericImage("alpine", "latest")
                       .with_hostname("test-hostname")
                       .with_cmd({"sh", "-c", "hostname"})
                       .start();

  EXPECT_THAT(container.stdout_to_string(), HasSubstr("test-hostname"));
}

// ============================================================================
// Filesystem Operations
// ============================================================================

TEST(GenericImageIntegrationTest, StartWithWorkingDir) {
  auto container = GenericImage("alpine", "latest")
                       .with_working_dir("/tmp")
                       .with_cmd({"sh", "-c", "pwd"})
                       .start();

  EXPECT_THAT(container.stdout_to_string(), HasSubstr("/tmp"));
}

TEST(GenericImageIntegrationTest, StartWithCopyTo) {

  // Create a temporary file with test content
  std::string test_content = "This is test content for copy operation";
  TempFile temp_file(test_content);

  auto container = GenericImage("alpine", "latest")
                       .with_copy_to("/tmp/test_file.txt", CopyDataSource::File(temp_file.path()))
                       .with_cmd({"sh", "-c", "cat /tmp/test_file.txt"})
                       .start();

  EXPECT_THAT(container.stdout_to_string(), HasSubstr(test_content));
}

TEST(GenericImageIntegrationTest, StartWithCopyToBytes) {
  std::string test_content = "Binary data test content";
  std::vector<std::uint8_t> data(test_content.begin(), test_content.end());

  auto container = GenericImage("alpine", "latest")
                       .with_copy_to("/tmp/binary_file.txt", CopyDataSource::Data(std::move(data)))
                       .with_cmd({"sh", "-c", "cat /tmp/binary_file.txt"})
                       .start();

  EXPECT_THAT(container.stdout_to_string(), HasSubstr(test_content));
}

// ============================================================================
// Security and Capabilities
// ============================================================================

TEST(GenericImageIntegrationTest, StartWithUser) {
  auto container = GenericImage("alpine", "latest")
                       .with_user("nobody")
                       .with_cmd({"sh", "-c", "whoami"})
                       .start();

  EXPECT_THAT(container.stdout_to_string(), HasSubstr("nobody"));
}

TEST(GenericImageIntegrationTest, StartWithPrivileged) {
  auto container = GenericImage("alpine", "latest")
                       .with_privileged(true)
                       .with_cmd({"sh", "-c", "sleep 200"})
                       .start();

  EXPECT_TRUE(container.is_running());
}

TEST(GenericImageIntegrationTest, StartWithCapAdd) {
  auto container = GenericImage("alpine", "latest")
                       .with_cap_add("NET_ADMIN")
                       .with_cmd({"sh", "-c", "sleep 200"})
                       .start();

  EXPECT_TRUE(container.is_running());
}

TEST(GenericImageIntegrationTest, StartWithCapDrop) {
  auto container = GenericImage("alpine", "latest")
                       .with_cap_drop("CHOWN")
                       .with_cmd({"sh", "-c", "sleep 200"})
                       .start();

  EXPECT_TRUE(container.is_running());
}

// ============================================================================
// Resource Limits
// ============================================================================

TEST(GenericImageIntegrationTest, StartWithShmSize) {

  std::uint64_t shm_size = 67108864; // 64MB

  auto container = GenericImage("alpine", "latest")
                       .with_shm_size(shm_size)
                       .with_cmd({"sh", "-c", "sleep 200"})
                       .start();

  EXPECT_TRUE(container.is_running());
}

TEST(GenericImageIntegrationTest, StartWithUlimit) {
  auto container = GenericImage("alpine", "latest")
                       .with_ulimit("nofile", 1024, 2048)
                       .with_cmd({"sh", "-c", "sleep 200"})
                       .start();

  EXPECT_TRUE(container.is_running());
}

// ============================================================================
// Wait Conditions
// ============================================================================

TEST(GenericImageIntegrationTest, StartWithLogWaitCondition) {
  auto wait_condition = WaitFor::message_on_stdout("Server started");

  auto container = GenericImage("alpine", "latest")
                       .with_wait_for(wait_condition)
                       .with_cmd({"sh", "-c", "echo 'Server started' && sleep 200"})
                       .start();

  EXPECT_TRUE(container.is_running());
}

TEST(GenericImageIntegrationTest, StartWithMultipleWaitConditions) {
  std::vector<WaitFor> conditions = {
      WaitFor::message_on_stdout("Line 1"),
      WaitFor::message_on_stdout("Line 2"),
  };

  auto container = GenericImage("alpine", "latest")
                       .with_ready_conditions(conditions)
                       .with_cmd({"sh", "-c", "echo 'Line 1' && echo 'Line 2' && sleep 200"})
                       .start();

  EXPECT_TRUE(container.is_running());
}

// ============================================================================
// Network Configuration
// ============================================================================

TEST(GenericImageIntegrationTest, StartWithHost) {
  auto container = GenericImage("alpine", "latest")
                       .with_host("test.local", Host::Addr("127.0.0.1"))
                       .with_cmd({"sh", "-c", "sleep 200"})
                       .start();

  EXPECT_TRUE(container.is_running());
}

// ============================================================================
// Container Lifecycle
// ============================================================================

TEST(GenericImageIntegrationTest, StartStopContainer) {
  auto container = GenericImage("alpine", "latest").with_cmd({"sh", "-c", "sleep 200"}).start();
  EXPECT_TRUE(container.is_running());

  container.stop();
  EXPECT_FALSE(container.is_running());
}

TEST(GenericImageIntegrationTest, StartStopWithTimeoutContainer) {
  auto container = GenericImage("alpine", "latest").with_cmd({"sh", "-c", "sleep 200"}).start();
  EXPECT_TRUE(container.is_running());

  container.stop_with_timeout(5); // 5 seconds timeout
  EXPECT_FALSE(container.is_running());
}

TEST(GenericImageIntegrationTest, StartStopStartContainer) {
  auto container = GenericImage("alpine", "latest").with_cmd({"sh", "-c", "sleep 200"}).start();
  EXPECT_TRUE(container.is_running());

  container.stop();
  EXPECT_FALSE(container.is_running());

  container.start();
  EXPECT_TRUE(container.is_running());
}

TEST(GenericImageIntegrationTest, ContainerExitCode) {
  auto container = GenericImage("alpine", "latest").with_cmd({"sh", "-c", "exit 42"}).start();

  // Wait for container to finish
  container.stdout_to_string();

  EXPECT_EQ(container.exit_code(), std::optional<std::int64_t>(42));
}

// ============================================================================
// Container Output
// ============================================================================

TEST(GenericImageIntegrationTest, ContainerStdout) {
  std::string expected_output = "Hello from Docker!";

  auto container = GenericImage("alpine", "latest")
                       .with_cmd({"sh", "-c", "echo '" + expected_output + "'"})
                       .start();

  EXPECT_THAT(container.stdout_to_string(), HasSubstr(expected_output));
}

TEST(GenericImageIntegrationTest, ContainerStderr) {
  std::string expected_error = "Error message";

  auto container = GenericImage("alpine", "latest")
                       .with_cmd({"sh", "-c", "echo '" + expected_error + "' >&2"})
                       .start();

  EXPECT_THAT(container.stderr_to_string(), HasSubstr(expected_error));
}

// ============================================================================
// Complex Scenarios
// ============================================================================

TEST(GenericImageIntegrationTest, ComplexConfiguration) {
  auto container = GenericImage("alpine", "latest")
                       .with_exposed_port(ContainerPort::Tcp(8080))
                       .with_container_name("complex_test_container")
                       .with_hostname("complex-host")
                       .with_env_var("VAR1", "value1")
                       .with_env_var("VAR2", "value2")
                       .with_label("test.key", "test.value")
                       .with_working_dir("/tmp")
                       .with_cmd({"sh", "-c", "sleep 200"})
                       .start();

  EXPECT_TRUE(container.is_running());
}

TEST(GenericImageIntegrationTest, ChainedFluentMethods) {
  auto container = GenericImage("alpine", "latest")
                       .with_env_var("TEST1", "val1")
                       .with_env_var("TEST2", "val2")
                       .with_env_var("TEST3", "val3")
                       .with_label("l1", "v1")
                       .with_label("l2", "v2")
                       .with_hostname("test-chain")
                       .with_working_dir("/usr")
                       .with_cmd({"sh", "-c", "sleep 200"})
                       .start();

  EXPECT_TRUE(container.is_running());
}

// ============================================================================
// Image Getters
// ============================================================================

TEST(GenericImageIntegrationTest, ImageGetters) {
  GenericImage image("alpine", "latest");

  EXPECT_EQ(image.name(), "alpine");
  EXPECT_EQ(image.tag(), "latest");
}

TEST(GenericImageIntegrationTest, ImageGettersWithConfiguration) {
  auto configured_image = GenericImage("alpine", "latest")
                              .with_exposed_port(ContainerPort::Tcp(8080))
                              .with_entrypoint("/bin/sh")
                              .with_wait_for(WaitFor::message_on_stdout("Ready"));

  EXPECT_EQ(configured_image.name(), "alpine");
  EXPECT_EQ(configured_image.tag(), "latest");

  auto entrypoint = configured_image.entrypoint();
  ASSERT_TRUE(entrypoint.has_value());
  EXPECT_EQ(entrypoint.value(), "/bin/sh");

  auto ports = configured_image.expose_ports();
  EXPECT_EQ(ports.size(), 1);
  EXPECT_EQ(ports[0].as_u16(), 8080);
  EXPECT_TRUE(ports[0].is_tcp());

  auto conditions = configured_image.ready_conditions();
  EXPECT_EQ(conditions.size(), 1);
}

// ============================================================================
// Pull Image
// ============================================================================

TEST(GenericImageIntegrationTest, PullImage) {
  // Pull the image explicitly
  auto pulled_image = GenericImage("alpine", "latest").pull();

  // Now start a container from the pulled image
  auto container = pulled_image.with_cmd({"sh", "-c", "sleep 200"}).start();

  EXPECT_TRUE(container.is_running());
}
