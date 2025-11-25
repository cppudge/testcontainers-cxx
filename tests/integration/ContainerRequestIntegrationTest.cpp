#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "testutils/TempFile.hpp"
#include <testcontainers/testcontainers.hpp>

using namespace testcontainers;
using namespace testcontainers::test_utils;

using ::testing::HasSubstr;

// ============================================================================
// Basic Container Request Operations
// ============================================================================

TEST(ContainerRequestIntegrationTest, BasicStartFromRequest) {
  auto container = GenericImage("alpine", "latest").with_cmd({"sh", "-c", "sleep 200"}).start();

  EXPECT_TRUE(container.is_running());
}

TEST(ContainerRequestIntegrationTest, RequestWithCustomImageName) {
  auto container = GenericImage("alpine", "3.18")
                       .with_name("alpine")
                       .with_tag("latest")
                       .with_cmd({"sh", "-c", "sleep 200"})
                       .start();

  EXPECT_TRUE(container.is_running());
}

// ============================================================================
// Platform Configuration
// ============================================================================

TEST(ContainerRequestIntegrationTest, RequestWithPlatform) {
  auto container = GenericImage("alpine", "latest")
                       .with_platform("linux/amd64")
                       .with_cmd({"sh", "-c", "sleep 200"})
                       .start();

  EXPECT_TRUE(container.is_running());
}

// ============================================================================
// Network Configuration
// ============================================================================

TEST(ContainerRequestIntegrationTest, RequestWithNetwork) {
  auto container = GenericImage("alpine", "latest")
                       .with_network("bridge")
                       .with_cmd({"sh", "-c", "sleep 200"})
                       .start();

  EXPECT_TRUE(container.is_running());
}

TEST(ContainerRequestIntegrationTest, RequestWithHostEntry) {
  auto container = GenericImage("alpine", "latest")
                       .with_host("testhost.local", Host::Addr("192.168.1.100"))
                       .with_cmd({"sh", "-c", "sleep 200"})
                       .start();

  EXPECT_TRUE(container.is_running());
}

TEST(ContainerRequestIntegrationTest, RequestWithMultipleHosts) {
  auto container = GenericImage("alpine", "latest")
                       .with_host("host1.local", Host::Addr("10.0.0.1"))
                       .with_host("host2.local", Host::Addr("10.0.0.2"))
                       .with_cmd({"sh", "-c", "sleep 200"})
                       .start();

  EXPECT_TRUE(container.is_running());
}

// ============================================================================
// Mount and Volume Configuration
// ============================================================================

TEST(ContainerRequestIntegrationTest, RequestWithMount) {
  auto temp_dir = std::filesystem::temp_directory_path() / "test_dir";
  std::filesystem::create_directories(temp_dir);
  std::ofstream(temp_dir / "test_file.txt") << "test content";

  auto container = GenericImage("alpine", "latest")
                       .with_mount(Mount::BindMount(temp_dir.string(), "/mnt/test"))
                       .with_cmd({"sh", "-c", "ls -la /mnt/test"})
                       .start();

  EXPECT_THAT(container.stdout_to_string(), HasSubstr("test_file.txt"));

  container.stop();
  std::filesystem::remove_all(temp_dir);
}

// ============================================================================
// Security and Capabilities
// ============================================================================

TEST(ContainerRequestIntegrationTest, RequestWithCgroupnsMode) {
  auto container = GenericImage("alpine", "latest")
                       .with_cgroupns_mode(CgroupnsMode::Host())
                       .with_cmd({"sh", "-c", "sleep 200"})
                       .start();

  EXPECT_TRUE(container.is_running());
}

TEST(ContainerRequestIntegrationTest, RequestWithUsernsMode) {
  auto container = GenericImage("alpine", "latest")
                       .with_userns_mode("host")
                       .with_cmd({"sh", "-c", "sleep 200"})
                       .start();

  EXPECT_TRUE(container.is_running());
}

TEST(ContainerRequestIntegrationTest, RequestWithReadonlyRootfs) {
  auto container = GenericImage("alpine", "latest")
                       .with_readonly_rootfs(true)
                       .with_cmd({"sh", "-c", "sleep 200"})
                       .start();

  EXPECT_TRUE(container.is_running());
}

TEST(ContainerRequestIntegrationTest, RequestWithSecurityOpt) {
  auto container = GenericImage("alpine", "latest")
                       .with_security_opt("no-new-privileges")
                       .with_cmd({"sh", "-c", "sleep 200"})
                       .start();

  EXPECT_TRUE(container.is_running());
}

TEST(ContainerRequestIntegrationTest, RequestWithMultipleSecurityOpts) {
  auto container = GenericImage("alpine", "latest")
                       .with_security_opt("no-new-privileges")
                       .with_security_opt("apparmor=unconfined")
                       .with_cmd({"sh", "-c", "sleep 200"})
                       .start();

  EXPECT_TRUE(container.is_running());
}

// ============================================================================
// Startup Timeout
// ============================================================================

TEST(ContainerRequestIntegrationTest, RequestWithStartupTimeout) {
  auto timeout = std::chrono::seconds(30);

  auto container = GenericImage("alpine", "latest")
                       .with_startup_timeout(timeout)
                       .with_cmd({"sh", "-c", "sleep 200"})
                       .start();

  EXPECT_TRUE(container.is_running());
}

// ============================================================================
// Health Check Configuration
// ============================================================================

TEST(ContainerRequestIntegrationTest, RequestWithHealthcheck) {
  auto healthcheck = Healthcheck::cmd({"CMD", "true"})
                         .with_interval(std::chrono::seconds(5))
                         .with_timeout(std::chrono::seconds(3))
                         .with_retries(3);

  auto container = GenericImage("alpine", "latest")
                       .with_health_check(std::move(healthcheck))
                       .with_cmd({"sh", "-c", "sleep 200"})
                       .start();

  EXPECT_TRUE(container.is_running());
}

TEST(ContainerRequestIntegrationTest, RequestWithComplexHealthcheck) {
  auto healthcheck = Healthcheck::cmd_shell("echo ok")
                         .with_interval(std::chrono::seconds(2))
                         .with_timeout(std::chrono::seconds(1))
                         .with_retries(5)
                         .with_start_period(std::chrono::seconds(1));

  auto container = GenericImage("alpine", "latest")
                       .with_health_check(std::move(healthcheck))
                       .with_cmd({"sh", "-c", "sleep 200"})
                       .start();

  EXPECT_TRUE(container.is_running());
}

// ============================================================================
// Copy Files Configuration
// ============================================================================

TEST(ContainerRequestIntegrationTest, RequestWithCopyFile) {
  std::string test_content = "Test file content for container";
  TempFile temp_file(test_content);

  auto container = GenericImage("alpine", "latest")
                       .with_copy_to("/tmp/copied_file.txt", CopyDataSource::File(temp_file.path()))
                       .with_cmd({"sh", "-c", "cat /tmp/copied_file.txt"})
                       .start();

  EXPECT_THAT(container.stdout_to_string(), HasSubstr(test_content));
}

TEST(ContainerRequestIntegrationTest, RequestWithCopyMultipleFiles) {
  std::string content1 = "File 1 content";
  std::string content2 = "File 2 content";

  TempFile temp_file1(content1);
  TempFile temp_file2(content2);

  auto container = GenericImage("alpine", "latest")
                       .with_copy_to("/tmp/file1.txt", CopyDataSource::File(temp_file1.path()))
                       .with_copy_to("/tmp/file2.txt", CopyDataSource::File(temp_file2.path()))
                       .with_cmd({"sh", "-c", "cat /tmp/file1.txt && cat /tmp/file2.txt"})
                       .start();

  auto stdout_str = container.stdout_to_string();
  EXPECT_THAT(stdout_str, HasSubstr(content1));
  EXPECT_THAT(stdout_str, HasSubstr(content2));
}

// ============================================================================
// Ready Conditions
// ============================================================================

TEST(ContainerRequestIntegrationTest, RequestWithSingleReadyCondition) {
  std::vector<WaitFor> conditions = {WaitFor::message_on_stdout("Container is ready")};

  auto container = GenericImage("alpine", "latest")
                       .with_ready_conditions(conditions)
                       .with_cmd({"sh", "-c", "echo 'Container is ready' && sleep 200"})
                       .start();

  EXPECT_TRUE(container.is_running());
}

TEST(ContainerRequestIntegrationTest, RequestWithMultipleReadyConditions) {
  std::vector<WaitFor> conditions = {
      WaitFor::message_on_stdout("Step 1 complete"),
      WaitFor::message_on_stdout("Step 2 complete"),
  };

  auto container = GenericImage("alpine", "latest")
                       .with_ready_conditions(conditions)
                       .with_cmd({
                           "sh",
                           "-c",
                           "echo 'Step 1 complete' && echo 'Step 2 complete' && sleep 200",
                       })
                       .start();

  EXPECT_TRUE(container.is_running());
}

// ============================================================================
// Pull Operation
// ============================================================================

TEST(ContainerRequestIntegrationTest, PullBeforeStart) {
  auto pulled_request = GenericImage("alpine", "latest").pull();

  auto container = pulled_request.with_cmd({"sh", "-c", "sleep 200"}).start();

  EXPECT_TRUE(container.is_running());
}

TEST(ContainerRequestIntegrationTest, PullWithConfiguration) {
  auto container = GenericImage("alpine", "latest")
                       .with_env_var("TEST_VAR", "test_value")
                       .with_label("test.label", "value")
                       .pull()
                       .with_cmd({"sh", "-c", "echo $TEST_VAR"})
                       .start();

  EXPECT_THAT(container.stdout_to_string(), HasSubstr("test_value"));
}

// ============================================================================
// Complex Configuration Chains
// ============================================================================

TEST(ContainerRequestIntegrationTest, CompleteConfigurationChain) {
  auto container = GenericImage("alpine", "latest")
                       .with_container_name("test_complex_request")
                       .with_hostname("test-host")
                       .with_env_var("VAR1", "value1")
                       .with_env_var("VAR2", "value2")
                       .with_label("app.name", "test-app")
                       .with_label("app.version", "1.0")
                       .with_working_dir("/tmp")
                       .with_user("nobody")
                       .with_privileged(false)
                       .with_shm_size(67108864) // 64MB
                       .with_cmd({"sh", "-c", "sleep 200"})
                       .start();

  EXPECT_TRUE(container.is_running());
}

TEST(ContainerRequestIntegrationTest, LongConfigurationChain) {
  std::string test_content = "Config file content";
  TempFile temp_file(test_content);

  auto container = GenericImage("alpine", "latest")
                       .with_container_name("test_long_chain")
                       .with_platform("linux/amd64")
                       .with_network("bridge")
                       .with_hostname("long-chain-host")
                       .with_env_var("ENV1", "val1")
                       .with_env_var("ENV2", "val2")
                       .with_env_var("ENV3", "val3")
                       .with_label("l1", "v1")
                       .with_label("l2", "v2")
                       .with_copy_to("/tmp/config.txt", CopyDataSource::File(temp_file.path()))
                       .with_working_dir("/tmp")
                       .with_ulimit("nofile", 1024, 2048)
                       .with_shm_size(134217728) // 128MB
                       .with_cmd({"sh", "-c", "cat /tmp/config.txt"})
                       .start();

  EXPECT_THAT(container.stdout_to_string(), HasSubstr(test_content));
}

// ============================================================================
// Mix GenericImage and ContainerRequest Methods
// ============================================================================

TEST(ContainerRequestIntegrationTest, MixImageAndRequestMethods) {
  // Configure via GenericImage
  auto configured_image = GenericImage("alpine", "latest")
                              .with_exposed_port(ContainerPort::Tcp(8080))
                              .with_entrypoint("/bin/sh");

  // Further configure via ContainerRequest
  auto container = configured_image.with_env_var("APP_PORT", "8080")
                       .with_label("app.type", "web")
                       .with_hostname("web-server")
                       .with_cmd({"-c", "sleep 200"})
                       .start();

  EXPECT_TRUE(container.is_running());
}

TEST(ContainerRequestIntegrationTest, BuildComplexImageConfiguration) {
  // Step 1: Configure image
  auto step1 = GenericImage("alpine", "latest")
                   .with_exposed_port(ContainerPort::Tcp(80))
                   .with_exposed_port(ContainerPort::Tcp(443));

  // Step 2: Add wait conditions
  auto step2 = step1.with_wait_for(WaitFor::message_on_stdout("Server ready"));

  // Step 3: Configure container
  auto step3 = step2.with_env_var("SERVER_PORT", "80")
                   .with_env_var("SSL_PORT", "443")
                   .with_hostname("web-container")
                   .with_label("service", "web");

  // Step 4: Start
  auto container = step3.with_cmd({"sh", "-c", "echo 'Server ready' && sleep 200"}).start();

  EXPECT_TRUE(container.is_running());
}

// ============================================================================
// Resource Limits and Constraints
// ============================================================================

TEST(ContainerRequestIntegrationTest, RequestWithMultipleUlimits) {
  // TODO: Fix this in testcontainers-rs .with_ulimit("nproc", 512, std::nullopt)
  // This will fail because of bug in testcontainers-rs
  auto container = GenericImage("alpine", "latest")
                       .with_ulimit("nofile", 1042, 2042)
                       .with_ulimit("nproc", 542, 2042)
                       .with_cmd({"/bin/sh", "-c", "ulimit -Sa && ulimit -Ha"})
                       .start();

  EXPECT_THAT(container.stdout_to_string(), HasSubstr("1042"));
  EXPECT_THAT(container.stdout_to_string(), HasSubstr("2042"));
  EXPECT_THAT(container.stdout_to_string(), HasSubstr("542"));
  EXPECT_THAT(container.stdout_to_string(), HasSubstr("2042"));
}

TEST(ContainerRequestIntegrationTest, RequestWithCapabilitiesAndPrivileges) {
  GenericImage image("alpine", "latest");

  auto container = image.with_cap_add("NET_ADMIN")
                       .with_cap_add("SYS_TIME")
                       .with_cap_drop("CHOWN")
                       .with_cmd({"sh", "-c", "sleep 200"})
                       .start();

  EXPECT_TRUE(container.is_running());
}

// ============================================================================
// Verification After Start
// ============================================================================

TEST(ContainerRequestIntegrationTest, VerifyEnvironmentVariables) {
  auto container = GenericImage("alpine", "latest")
                       .with_env_var("TEST1", "value1")
                       .with_env_var("TEST2", "value2")
                       .with_env_var("TEST3", "value3")
                       .with_cmd({"sh", "-c", "env | grep TEST"})
                       .start();

  auto stdout_str = container.stdout_to_string();
  EXPECT_THAT(stdout_str, HasSubstr("TEST1=value1"));
  EXPECT_THAT(stdout_str, HasSubstr("TEST2=value2"));
  EXPECT_THAT(stdout_str, HasSubstr("TEST3=value3"));
}

TEST(ContainerRequestIntegrationTest, VerifyWorkingDirectory) {
  auto container = GenericImage("alpine", "latest")
                       .with_working_dir("/usr/local")
                       .with_cmd({"sh", "-c", "pwd"})
                       .start();

  EXPECT_THAT(container.stdout_to_string(), HasSubstr("/usr/local"));
}

TEST(ContainerRequestIntegrationTest, VerifyUser) {
  auto container = GenericImage("alpine", "latest")
                       .with_user("nobody")
                       .with_cmd({"sh", "-c", "id -un"})
                       .start();

  EXPECT_THAT(container.stdout_to_string(), HasSubstr("nobody"));
}
