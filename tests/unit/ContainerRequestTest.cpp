#include <chrono>
#include <vector>

#include <gtest/gtest.h>

#include <testcontainers/testcontainers.hpp>

#include "testutils/TempFile.hpp"

using namespace testcontainers;
using namespace testcontainers::test_utils;

// Хелпер для создания ContainerRequest из GenericImage
static ContainerRequest create_request() {
  return GenericImage("alpine", "latest").with_cmd({"echo", "test"});
}

// ====================
// with_cmd Tests
// ====================

TEST(ContainerRequestTest, WithCmdSingleArg) {
  auto result = GenericImage("alpine", "latest").with_cmd({"echo"});
  EXPECT_TRUE(result.is_valid());
}

TEST(ContainerRequestTest, WithCmdMultipleArgs) {
  auto result = GenericImage("alpine", "latest").with_cmd({"echo", "hello", "world"});
  EXPECT_TRUE(result.is_valid());
}

TEST(ContainerRequestTest, WithCmdEmpty) {
  auto result = GenericImage("alpine", "latest").with_cmd({});
  EXPECT_TRUE(result.is_valid());
}

TEST(ContainerRequestTest, WithCmdComplexCommand) {
  auto result = GenericImage("alpine", "latest").with_cmd({"/bin/sh", "-c", "echo test && sleep 1"});
  EXPECT_TRUE(result.is_valid());
}

// ====================
// with_name Tests
// ====================

TEST(ContainerRequestTest, WithName) {
  auto result = create_request().with_name("redis");
  EXPECT_TRUE(result.is_valid());
}

TEST(ContainerRequestTest, WithNameEmpty) {
  auto result = create_request().with_name("");
  EXPECT_TRUE(result.is_valid());
}

// ====================
// with_tag Tests
// ====================

TEST(ContainerRequestTest, WithTag) {
  auto result = create_request().with_tag("7.2.4");
  EXPECT_TRUE(result.is_valid());
}

TEST(ContainerRequestTest, WithTagLatest) {
  auto result = create_request().with_tag("latest");
  EXPECT_TRUE(result.is_valid());
}

// ====================
// with_container_name Tests
// ====================

TEST(ContainerRequestTest, WithContainerName) {
  auto result = create_request().with_container_name("my-test-container");
  EXPECT_TRUE(result.is_valid());
}

TEST(ContainerRequestTest, WithContainerNameWithSpecialChars) {
  auto result = create_request().with_container_name("my_test-container.123");
  EXPECT_TRUE(result.is_valid());
}

// ====================
// with_platform Tests
// ====================

TEST(ContainerRequestTest, WithPlatformLinuxAmd64) {
  auto result = create_request().with_platform("linux/amd64");
  EXPECT_TRUE(result.is_valid());
}

TEST(ContainerRequestTest, WithPlatformLinuxArm64) {
  auto result = create_request().with_platform("linux/arm64");
  EXPECT_TRUE(result.is_valid());
}

// ====================
// with_network Tests
// ====================

TEST(ContainerRequestTest, WithNetworkBridge) {
  auto result = create_request().with_network("bridge");
  EXPECT_TRUE(result.is_valid());
}

TEST(ContainerRequestTest, WithNetworkHost) {
  auto result = create_request().with_network("host");
  EXPECT_TRUE(result.is_valid());
}

TEST(ContainerRequestTest, WithNetworkNone) {
  auto result = create_request().with_network("none");
  EXPECT_TRUE(result.is_valid());
}

// ====================
// with_label Tests
// ====================

TEST(ContainerRequestTest, WithLabel) {
  auto result = create_request().with_label("version", "1.0.0");
  EXPECT_TRUE(result.is_valid());
}

TEST(ContainerRequestTest, WithLabelEmpty) {
  auto result = create_request().with_label("", "");
  EXPECT_TRUE(result.is_valid());
}

TEST(ContainerRequestTest, WithMultipleLabels) {
  auto result = create_request()
                    .with_label("env", "production")
                    .with_label("version", "1.0.0")
                    .with_label("team", "backend");
  EXPECT_TRUE(result.is_valid());
}

// ====================
// with_labels Tests
// ====================

TEST(ContainerRequestTest, WithLabelsMultiple) {
  std::vector<std::pair<std::string, std::string>> labels = {
      {"env", "production"}, {"version", "1.0.0"}, {"team", "backend"}};
  auto result = create_request().with_labels(labels);
  EXPECT_TRUE(result.is_valid());
}

TEST(ContainerRequestTest, WithLabelsEmpty) {
  auto result = create_request().with_labels({});
  EXPECT_TRUE(result.is_valid());
}

TEST(ContainerRequestTest, WithLabelsSingle) {
  std::vector<std::pair<std::string, std::string>> labels = {{"app", "test"}};
  auto result = create_request().with_labels(labels);
  EXPECT_TRUE(result.is_valid());
}

// ====================
// with_host Tests
// ====================

TEST(ContainerRequestTest, WithHostAddr) {
  auto result = create_request().with_host("database", Host::Addr("10.0.0.5"));
  EXPECT_TRUE(result.is_valid());
}

TEST(ContainerRequestTest, WithHostGateway) {
  auto result = create_request().with_host("host.docker.internal", Host::HostGatewayLinux());
  EXPECT_TRUE(result.is_valid());
}

TEST(ContainerRequestTest, WithMultipleHosts) {
  auto result = create_request()
                    .with_host("db", Host::Addr("10.0.0.1"))
                    .with_host("cache", Host::Addr("10.0.0.2"));
  EXPECT_TRUE(result.is_valid());
}

// ====================
// with_mount Tests
// ====================

TEST(ContainerRequestTest, WithMountBind) {
  auto result = create_request().with_mount(Mount::BindMount("/host/path", "/container/path"));
  EXPECT_TRUE(result.is_valid());
}

TEST(ContainerRequestTest, WithMountVolume) {
  auto result = create_request().with_mount(Mount::VolumeMount("myvolume", "/data"));
  EXPECT_TRUE(result.is_valid());
}

TEST(ContainerRequestTest, WithMountTmpfs) {
  auto result = create_request().with_mount(Mount::TmpfsMount("/tmp"));
  EXPECT_TRUE(result.is_valid());
}

TEST(ContainerRequestTest, WithMountReadOnly) {
  auto result =
      create_request().with_mount(Mount::BindMount("/host/config", "/app/config").with_read_only());
  EXPECT_TRUE(result.is_valid());
}

TEST(ContainerRequestTest, WithMultipleMounts) {
  auto result = create_request()
                    .with_mount(Mount::BindMount("/host/data", "/app/data"))
                    .with_mount(Mount::VolumeMount("logs", "/var/log"))
                    .with_mount(Mount::TmpfsMount("/tmp"));
  EXPECT_TRUE(result.is_valid());
}

// ====================
// with_env_var Tests
// ====================

TEST(ContainerRequestTest, WithEnvVar) {
  auto result = create_request().with_env_var("DEBUG", "true");
  EXPECT_TRUE(result.is_valid());
}

TEST(ContainerRequestTest, WithEnvVarEmpty) {
  auto result = create_request().with_env_var("EMPTY", "");
  EXPECT_TRUE(result.is_valid());
}

TEST(ContainerRequestTest, WithMultipleEnvVars) {
  auto result = create_request()
                    .with_env_var("ENV", "production")
                    .with_env_var("DEBUG", "false")
                    .with_env_var("PORT", "8080");
  EXPECT_TRUE(result.is_valid());
}

// ====================
// with_hostname Tests
// ====================

TEST(ContainerRequestTest, WithHostname) {
  auto result = create_request().with_hostname("myapp.local");
  EXPECT_TRUE(result.is_valid());
}

TEST(ContainerRequestTest, WithHostnameFQDN) {
  auto result = create_request().with_hostname("app.example.com");
  EXPECT_TRUE(result.is_valid());
}

// ====================
// with_mapped_port Tests
// ====================

TEST(ContainerRequestTest, WithMappedPortSame) {
  auto result = create_request().with_mapped_port(8080, ContainerPort::Tcp(8080));
  EXPECT_TRUE(result.is_valid());
}

TEST(ContainerRequestTest, WithMappedPortDifferent) {
  auto result = create_request().with_mapped_port(8080, ContainerPort::Tcp(80));
  EXPECT_TRUE(result.is_valid());
}

TEST(ContainerRequestTest, WithMappedPortUdp) {
  auto result = create_request().with_mapped_port(53, ContainerPort::Udp(53));
  EXPECT_TRUE(result.is_valid());
}

TEST(ContainerRequestTest, WithMultipleMappedPorts) {
  auto result = create_request()
                    .with_mapped_port(8080, ContainerPort::Tcp(80))
                    .with_mapped_port(8443, ContainerPort::Tcp(443));
  EXPECT_TRUE(result.is_valid());
}

// ====================
// with_privileged Tests
// ====================

TEST(ContainerRequestTest, WithPrivilegedTrue) {
  auto result = create_request().with_privileged(true);
  EXPECT_TRUE(result.is_valid());
}

TEST(ContainerRequestTest, WithPrivilegedFalse) {
  auto result = create_request().with_privileged(false);
  EXPECT_TRUE(result.is_valid());
}

// ====================
// with_cap_add Tests
// ====================

TEST(ContainerRequestTest, WithCapAdd) {
  auto result = create_request().with_cap_add("NET_ADMIN");
  EXPECT_TRUE(result.is_valid());
}

TEST(ContainerRequestTest, WithMultipleCapAdd) {
  auto result = create_request()
                    .with_cap_add("NET_ADMIN")
                    .with_cap_add("SYS_TIME")
                    .with_cap_add("SYS_PTRACE");
  EXPECT_TRUE(result.is_valid());
}

// ====================
// with_cap_drop Tests
// ====================

TEST(ContainerRequestTest, WithCapDrop) {
  auto result = create_request().with_cap_drop("NET_RAW");
  EXPECT_TRUE(result.is_valid());
}

TEST(ContainerRequestTest, WithMultipleCapDrop) {
  auto result =
      create_request().with_cap_drop("NET_RAW").with_cap_drop("SYS_ADMIN").with_cap_drop("MKNOD");
  EXPECT_TRUE(result.is_valid());
}

// ====================
// with_cgroupns_mode Tests
// ====================

TEST(ContainerRequestTest, WithCgroupnsModePrivate) {
  auto result = create_request().with_cgroupns_mode(CgroupnsMode::Private());
  EXPECT_TRUE(result.is_valid());
}

TEST(ContainerRequestTest, WithCgroupnsModeHost) {
  auto result = create_request().with_cgroupns_mode(CgroupnsMode::Host());
  EXPECT_TRUE(result.is_valid());
}

// ====================
// with_userns_mode Tests
// ====================

TEST(ContainerRequestTest, WithUsernsMode) {
  auto result = create_request().with_userns_mode("host");
  EXPECT_TRUE(result.is_valid());
}

TEST(ContainerRequestTest, WithUsernsModeEmpty) {
  auto result = create_request().with_userns_mode("");
  EXPECT_TRUE(result.is_valid());
}

// ====================
// with_shm_size Tests
// ====================

TEST(ContainerRequestTest, WithShmSize) {
  auto result = create_request().with_shm_size(64 * 1024 * 1024); // 64 MB
  EXPECT_TRUE(result.is_valid());
}

TEST(ContainerRequestTest, WithShmSizeZero) {
  auto result = create_request().with_shm_size(0);
  EXPECT_TRUE(result.is_valid());
}

TEST(ContainerRequestTest, WithShmSizeLarge) {
  auto result = create_request().with_shm_size(1024ULL * 1024 * 1024); // 1 GB
  EXPECT_TRUE(result.is_valid());
}

// ====================
// with_startup_timeout Tests
// ====================

TEST(ContainerRequestTest, WithStartupTimeout) {
  auto result = create_request().with_startup_timeout(std::chrono::seconds(30));
  EXPECT_TRUE(result.is_valid());
}

TEST(ContainerRequestTest, WithStartupTimeoutMillis) {
  auto result = create_request().with_startup_timeout(std::chrono::milliseconds(5000));
  EXPECT_TRUE(result.is_valid());
}

TEST(ContainerRequestTest, WithStartupTimeoutMinutes) {
  auto result = create_request().with_startup_timeout(std::chrono::minutes(5));
  EXPECT_TRUE(result.is_valid());
}

// ====================
// with_working_dir Tests
// ====================

TEST(ContainerRequestTest, WithWorkingDir) {
  auto result = create_request().with_working_dir("/app");
  EXPECT_TRUE(result.is_valid());
}

TEST(ContainerRequestTest, WithWorkingDirRoot) {
  auto result = create_request().with_working_dir("/");
  EXPECT_TRUE(result.is_valid());
}

TEST(ContainerRequestTest, WithWorkingDirNested) {
  auto result = create_request().with_working_dir("/usr/local/app/bin");
  EXPECT_TRUE(result.is_valid());
}

// ====================
// with_user Tests
// ====================

TEST(ContainerRequestTest, WithUserName) {
  auto result = create_request().with_user("nobody");
  EXPECT_TRUE(result.is_valid());
}

TEST(ContainerRequestTest, WithUserUid) {
  auto result = create_request().with_user("1000");
  EXPECT_TRUE(result.is_valid());
}

TEST(ContainerRequestTest, WithUserUidGid) {
  auto result = create_request().with_user("1000:1000");
  EXPECT_TRUE(result.is_valid());
}

TEST(ContainerRequestTest, WithUserRoot) {
  auto result = create_request().with_user("root");
  EXPECT_TRUE(result.is_valid());
}

// ====================
// with_readonly_rootfs Tests
// ====================

TEST(ContainerRequestTest, WithReadonlyRootfsTrue) {
  auto result = create_request().with_readonly_rootfs(true);
  EXPECT_TRUE(result.is_valid());
}

TEST(ContainerRequestTest, WithReadonlyRootfsFalse) {
  auto result = create_request().with_readonly_rootfs(false);
  EXPECT_TRUE(result.is_valid());
}

// ====================
// with_security_opt Tests
// ====================

TEST(ContainerRequestTest, WithSecurityOpt) {
  auto result = create_request().with_security_opt("no-new-privileges");
  EXPECT_TRUE(result.is_valid());
}

TEST(ContainerRequestTest, WithSecurityOptApparmor) {
  auto result = create_request().with_security_opt("apparmor=unconfined");
  EXPECT_TRUE(result.is_valid());
}

TEST(ContainerRequestTest, WithSecurityOptSeccomp) {
  auto result = create_request().with_security_opt("seccomp=unconfined");
  EXPECT_TRUE(result.is_valid());
}

TEST(ContainerRequestTest, WithMultipleSecurityOpts) {
  auto result = create_request()
                    .with_security_opt("no-new-privileges")
                    .with_security_opt("seccomp=unconfined");
  EXPECT_TRUE(result.is_valid());
}

// ====================
// with_ready_conditions Tests
// ====================

TEST(ContainerRequestTest, WithReadyConditionsSingle) {
  std::vector<WaitFor> conditions = {WaitFor::message_on_stdout("Ready")};
  auto result = create_request().with_ready_conditions(conditions);
  EXPECT_TRUE(result.is_valid());
}

TEST(ContainerRequestTest, WithReadyConditionsMultiple) {
  std::vector<WaitFor> conditions = {WaitFor::message_on_stdout("Ready"),
                                     WaitFor::healthcheck(), WaitFor::seconds(5)};
  auto result = create_request().with_ready_conditions(conditions);
  EXPECT_TRUE(result.is_valid());
}

TEST(ContainerRequestTest, WithReadyConditionsEmpty) {
  auto result = create_request().with_ready_conditions({});
  EXPECT_TRUE(result.is_valid());
}

// ====================
// with_copy_to Tests
// ====================

TEST(ContainerRequestTest, WithCopyToFile) {
  TempFile source("config content");
  auto result = create_request().with_copy_to("/app/config.txt", CopyDataSource::File(source.path()));
  EXPECT_TRUE(result.is_valid());
}

TEST(ContainerRequestTest, WithCopyToData) {
  std::vector<std::uint8_t> data = {0x48, 0x65, 0x6C, 0x6C, 0x6F}; // "Hello"
  auto result = create_request().with_copy_to("/app/data.bin", CopyDataSource::Data(data));
  EXPECT_TRUE(result.is_valid());
}

TEST(ContainerRequestTest, WithMultipleCopyTo) {
  TempFile file1("config1");
  TempFile file2("config2");
  auto result = create_request()
                    .with_copy_to("/app/config1.txt", CopyDataSource::File(file1.path()))
                    .with_copy_to("/app/config2.txt", CopyDataSource::File(file2.path()));
  EXPECT_TRUE(result.is_valid());
}

// ====================
// with_ulimit Tests
// ====================

TEST(ContainerRequestTest, WithUlimitNoHard) {
  auto result = create_request().with_ulimit("nofile", 1024, std::nullopt);
  EXPECT_TRUE(result.is_valid());
}

TEST(ContainerRequestTest, WithUlimitWithHard) {
  auto result = create_request().with_ulimit("nofile", 1024, 2048);
  EXPECT_TRUE(result.is_valid());
}

TEST(ContainerRequestTest, WithMultipleUlimits) {
  auto result = create_request()
                    .with_ulimit("nofile", 1024, 2048)
                    .with_ulimit("nproc", 512, std::nullopt);
  EXPECT_TRUE(result.is_valid());
}

// ====================
// with_health_check Tests
// ====================

TEST(ContainerRequestTest, WithHealthCheckCmdShell) {
  auto result =
      create_request().with_health_check(Healthcheck::cmd_shell("curl -f http://localhost/ || exit 1"));
  EXPECT_TRUE(result.is_valid());
}

TEST(ContainerRequestTest, WithHealthCheckCmd) {
  auto result =
      create_request().with_health_check(Healthcheck::cmd({"curl", "-f", "http://localhost/"}));
  EXPECT_TRUE(result.is_valid());
}

TEST(ContainerRequestTest, WithHealthCheckNone) {
  auto result = create_request().with_health_check(Healthcheck::none());
  EXPECT_TRUE(result.is_valid());
}

TEST(ContainerRequestTest, WithHealthCheckEmpty) {
  auto result = create_request().with_health_check(Healthcheck::empty());
  EXPECT_TRUE(result.is_valid());
}

TEST(ContainerRequestTest, WithHealthCheckWithOptions) {
  auto result =
      create_request().with_health_check(Healthcheck::cmd_shell("test -f /ready")
                                             .with_interval(std::chrono::seconds(5))
                                             .with_timeout(std::chrono::seconds(3))
                                             .with_retries(3));
  EXPECT_TRUE(result.is_valid());
}

// ====================
// Complex Fluent Chain Tests
// ====================

TEST(ContainerRequestTest, ComplexFluentChainBasic) {
  auto result = create_request()
                    .with_name("myapp")
                    .with_tag("v1.0")
                    .with_env_var("ENV", "production")
                    .with_working_dir("/app")
                    .with_user("nobody");
  EXPECT_TRUE(result.is_valid());
}

TEST(ContainerRequestTest, ComplexFluentChainAdvanced) {
  auto result = create_request()
                    .with_container_name("test-container")
                    .with_platform("linux/amd64")
                    .with_network("bridge")
                    .with_label("app", "test")
                    .with_env_var("DEBUG", "true")
                    .with_hostname("test.local")
                    .with_mapped_port(8080, ContainerPort::Tcp(80))
                    .with_working_dir("/app")
                    .with_user("1000:1000");
  EXPECT_TRUE(result.is_valid());
}

TEST(ContainerRequestTest, ComplexFluentChainWithMountsAndHealth) {
  TempFile config("server config");
  auto result = create_request()
                    .with_mount(Mount::BindMount("/host/data", "/app/data"))
                    .with_mount(Mount::TmpfsMount("/tmp"))
                    .with_copy_to("/app/config.txt", CopyDataSource::File(config.path()))
                    .with_health_check(Healthcheck::cmd_shell("test -f /ready")
                                           .with_interval(std::chrono::seconds(5)))
                    .with_ready_conditions({WaitFor::healthcheck(), WaitFor::seconds(2)});
  EXPECT_TRUE(result.is_valid());
}

TEST(ContainerRequestTest, ComplexFluentChainSecurity) {
  auto result = create_request()
                    .with_privileged(false)
                    .with_cap_add("NET_ADMIN")
                    .with_cap_drop("NET_RAW")
                    .with_security_opt("no-new-privileges")
                    .with_readonly_rootfs(true)
                    .with_cgroupns_mode(CgroupnsMode::Private());
  EXPECT_TRUE(result.is_valid());
}

// ====================
// Move Semantics Tests
// ====================

TEST(ContainerRequestTest, MoveConstructor) {
  auto request1 = create_request();
  auto request2 = std::move(request1);
  EXPECT_TRUE(request2.is_valid());
}

TEST(ContainerRequestTest, MoveAssignment) {
  auto request1 = create_request();
  auto request2 = create_request().with_name("other");
  request2 = std::move(request1);
  EXPECT_TRUE(request2.is_valid());
}

// ====================
// Edge Cases Tests
// ====================

TEST(ContainerRequestTest, VeryLongStrings) {
  std::string long_name(1000, 'a');
  std::string long_value(5000, 'b');
  auto result = create_request().with_env_var(long_name, long_value);
  EXPECT_TRUE(result.is_valid());
}

TEST(ContainerRequestTest, UnicodeInStrings) {
  auto result = create_request()
                    .with_env_var("MESSAGE", "Привет мир! 你好世界! 🚀")
                    .with_label("description", "Тестовое приложение");
  EXPECT_TRUE(result.is_valid());
}

TEST(ContainerRequestTest, EmptyStringsInVariousMethods) {
  auto result = create_request()
                    .with_name("")
                    .with_tag("")
                    .with_hostname("")
                    .with_working_dir("")
                    .with_user("");
  EXPECT_TRUE(result.is_valid());
}

TEST(ContainerRequestTest, SpecialCharactersInLabels) {
  auto result = create_request()
                    .with_label("com.example.version", "1.0.0-alpha+build.123")
                    .with_label("io.github.project/name", "test-app");
  EXPECT_TRUE(result.is_valid());
}

TEST(ContainerRequestTest, LargeBinaryData) {
  std::vector<std::uint8_t> large_data(1024 * 1024, 0xFF); // 1 MB
  auto result = create_request().with_copy_to("/app/large.bin", CopyDataSource::Data(large_data));
  EXPECT_TRUE(result.is_valid());
}

TEST(ContainerRequestTest, ManyLabels) {
  std::vector<std::pair<std::string, std::string>> labels;
  for (int i = 0; i < 50; ++i) {
    labels.push_back({"key" + std::to_string(i), "value" + std::to_string(i)});
  }
  auto result = create_request().with_labels(labels);
  EXPECT_TRUE(result.is_valid());
}

TEST(ContainerRequestTest, ManyMounts) {
  auto request = create_request();
  for (int i = 0; i < 10; ++i) {
    request = request.with_mount(
        Mount::BindMount("/host/path" + std::to_string(i), "/container/path" + std::to_string(i)));
  }
  EXPECT_TRUE(request.is_valid());
}

