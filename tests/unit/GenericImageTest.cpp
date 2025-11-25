#include <chrono>
#include <vector>

#include <gtest/gtest.h>

#include <testcontainers/testcontainers.hpp>

#include "testutils/TempFile.hpp"

using namespace testcontainers;
using namespace testcontainers::test_utils;

// ====================
// Constructor Tests
// ====================

TEST(GenericImageTest, ConstructorWithValidNameAndTag) {
  GenericImage image("redis", "7.2.4");
  EXPECT_TRUE(image.is_valid());
  EXPECT_EQ(image.name(), "redis");
  EXPECT_EQ(image.tag(), "7.2.4");
  EXPECT_FALSE(image.entrypoint().has_value());
  EXPECT_TRUE(image.expose_ports().empty());
  EXPECT_TRUE(image.ready_conditions().empty());
}

TEST(GenericImageTest, ConstructorWithLatestTag) {
  GenericImage image("nginx", "latest");
  EXPECT_TRUE(image.is_valid());
}

TEST(GenericImageTest, ConstructorWithEmptyName) {
  GenericImage image("", "latest");
  EXPECT_TRUE(image.is_valid());
}

TEST(GenericImageTest, ConstructorWithEmptyTag) {
  GenericImage image("alpine", "");
  EXPECT_TRUE(image.is_valid());
}

// ====================
// IImage interface - with_exposed_port Tests
// ====================

TEST(GenericImageTest, WithExposedPortTcp) {
  auto result = GenericImage("redis", "latest").with_exposed_port(ContainerPort::Tcp(6379));
  EXPECT_TRUE(result.is_valid());
  auto ports = result.expose_ports();
  EXPECT_EQ(ports.size(), 1);
  EXPECT_EQ(ports[0].as_u16(), 6379);
  EXPECT_TRUE(ports[0].is_tcp());
}

TEST(GenericImageTest, WithExposedPortUdp) {
  auto result = GenericImage("app", "latest").with_exposed_port(ContainerPort::Udp(5353));
  EXPECT_TRUE(result.is_valid());
}

TEST(GenericImageTest, WithExposedPortSctp) {
  auto result = GenericImage("app", "latest").with_exposed_port(ContainerPort::Sctp(9899));
  EXPECT_TRUE(result.is_valid());
}

TEST(GenericImageTest, WithMultipleExposedPorts) {
  auto result = GenericImage("app", "latest")
                    .with_exposed_port(ContainerPort::Tcp(8080))
                    .with_exposed_port(ContainerPort::Tcp(8081))
                    .with_exposed_port(ContainerPort::Udp(53));
  EXPECT_TRUE(result.is_valid());
  auto ports = result.expose_ports();
  EXPECT_EQ(ports.size(), 3);
  EXPECT_EQ(ports[0].as_u16(), 8080);
  EXPECT_TRUE(ports[0].is_tcp());
  EXPECT_EQ(ports[1].as_u16(), 8081);
  EXPECT_TRUE(ports[1].is_tcp());
  EXPECT_EQ(ports[2].as_u16(), 53);
  EXPECT_TRUE(ports[2].is_udp());
}

// ====================
// IImage interface - with_entrypoint Tests
// ====================

TEST(GenericImageTest, WithEntrypointSimple) {
  auto result = GenericImage("alpine", "latest").with_entrypoint("/bin/sh");
  EXPECT_TRUE(result.is_valid());
  EXPECT_EQ(result.entrypoint(), std::optional<std::string>("/bin/sh"));
}

TEST(GenericImageTest, WithEntrypointWithArgs) {
  auto result = GenericImage("alpine", "latest").with_entrypoint("/bin/sh -c 'echo hello'");
  EXPECT_TRUE(result.is_valid());
}

TEST(GenericImageTest, WithEntrypointEmpty) {
  auto result = GenericImage("alpine", "latest").with_entrypoint("");
  EXPECT_TRUE(result.is_valid());
}

// ====================
// IImage interface - with_wait_for Tests
// ====================

TEST(GenericImageTest, WithWaitForNothing) {
  auto result = GenericImage("alpine", "latest").with_wait_for(WaitFor::Nothing());
  EXPECT_TRUE(result.is_valid());
}

TEST(GenericImageTest, WithWaitForSeconds) {
  auto result = GenericImage("redis", "latest").with_wait_for(WaitFor::seconds(5));
  EXPECT_TRUE(result.is_valid());
}

TEST(GenericImageTest, WithWaitForMillis) {
  auto result = GenericImage("redis", "latest").with_wait_for(WaitFor::millis(500));
  EXPECT_TRUE(result.is_valid());
}

TEST(GenericImageTest, WithWaitForHealthcheck) {
  auto result = GenericImage("postgres", "latest").with_wait_for(WaitFor::healthcheck());
  EXPECT_TRUE(result.is_valid());
}

TEST(GenericImageTest, WithWaitForExit) {
  auto result = GenericImage("alpine", "latest").with_wait_for(WaitFor::exit());
  EXPECT_TRUE(result.is_valid());
}

TEST(GenericImageTest, WithWaitForMessageOnStdout) {
  auto result = GenericImage("redis", "latest").with_wait_for(WaitFor::message_on_stdout("Ready"));
  EXPECT_TRUE(result.is_valid());
}

TEST(GenericImageTest, WithWaitForMessageOnStderr) {
  auto result = GenericImage("app", "latest").with_wait_for(WaitFor::message_on_stderr("Error"));
  EXPECT_TRUE(result.is_valid());
}

TEST(GenericImageTest, WithWaitForMessageOnEitherStd) {
  auto result =
      GenericImage("app", "latest").with_wait_for(WaitFor::message_on_either_std("Starting"));
  EXPECT_TRUE(result.is_valid());
}

// ====================
// IImageExt interface - Transition to ContainerRequest Tests
// ====================

TEST(GenericImageTest, WithCmd) {
  auto result = GenericImage("alpine", "latest").with_cmd({"echo", "hello"});
  EXPECT_TRUE(result.is_valid());
}

TEST(GenericImageTest, WithCmdEmpty) {
  auto result = GenericImage("alpine", "latest").with_cmd({});
  EXPECT_TRUE(result.is_valid());
}

TEST(GenericImageTest, WithCmdMultipleArgs) {
  auto result =
      GenericImage("alpine", "latest").with_cmd({"/bin/sh", "-c", "echo hello && sleep 1"});
  EXPECT_TRUE(result.is_valid());
}

TEST(GenericImageTest, WithName) {
  auto result = GenericImage("redis", "7.0").with_name("redis");
  EXPECT_TRUE(result.is_valid());
}

TEST(GenericImageTest, WithTag) {
  auto result = GenericImage("redis", "7.0").with_tag("7.2.4");
  EXPECT_TRUE(result.is_valid());
}

TEST(GenericImageTest, WithContainerName) {
  auto result = GenericImage("redis", "latest").with_container_name("my-redis-container");
  EXPECT_TRUE(result.is_valid());
}

TEST(GenericImageTest, WithPlatform) {
  auto result = GenericImage("alpine", "latest").with_platform("linux/amd64");
  EXPECT_TRUE(result.is_valid());
}

TEST(GenericImageTest, WithPlatformArm) {
  auto result = GenericImage("alpine", "latest").with_platform("linux/arm64");
  EXPECT_TRUE(result.is_valid());
}

TEST(GenericImageTest, WithNetwork) {
  auto result = GenericImage("redis", "latest").with_network("bridge");
  EXPECT_TRUE(result.is_valid());
}

TEST(GenericImageTest, WithNetworkHost) {
  auto result = GenericImage("nginx", "latest").with_network("host");
  EXPECT_TRUE(result.is_valid());
}

TEST(GenericImageTest, WithLabel) {
  auto result = GenericImage("app", "latest").with_label("version", "1.0.0");
  EXPECT_TRUE(result.is_valid());
}

TEST(GenericImageTest, WithLabels) {
  std::vector<std::pair<std::string, std::string>> labels = {
      {"env", "production"}, {"version", "1.0.0"}, {"team", "backend"}};
  auto result = GenericImage("app", "latest").with_labels(labels);
  EXPECT_TRUE(result.is_valid());
}

TEST(GenericImageTest, WithLabelsEmpty) {
  auto result = GenericImage("app", "latest").with_labels({});
  EXPECT_TRUE(result.is_valid());
}

TEST(GenericImageTest, WithHost) {
  auto result = GenericImage("app", "latest").with_host("database", Host::Addr("10.0.0.5"));
  EXPECT_TRUE(result.is_valid());
}

TEST(GenericImageTest, WithHostGateway) {
  auto result =
      GenericImage("app", "latest").with_host("host.docker.internal", Host::HostGatewayLinux());
  EXPECT_TRUE(result.is_valid());
}

TEST(GenericImageTest, WithMount) {
  auto result =
      GenericImage("app", "latest").with_mount(Mount::BindMount("/host/path", "/container/path"));
  EXPECT_TRUE(result.is_valid());
}

TEST(GenericImageTest, WithMountVolume) {
  auto result = GenericImage("postgres", "latest")
                    .with_mount(Mount::VolumeMount("pgdata", "/var/lib/postgresql/data"));
  EXPECT_TRUE(result.is_valid());
}

TEST(GenericImageTest, WithMountTmpfs) {
  auto result = GenericImage("app", "latest").with_mount(Mount::TmpfsMount("/tmp"));
  EXPECT_TRUE(result.is_valid());
}

TEST(GenericImageTest, WithMountReadOnly) {
  auto result = GenericImage("app", "latest")
                    .with_mount(Mount::BindMount("/host/config", "/app/config").with_read_only());
  EXPECT_TRUE(result.is_valid());
}

TEST(GenericImageTest, WithEnvVar) {
  auto result = GenericImage("redis", "latest").with_env_var("REDIS_PASSWORD", "secret");
  EXPECT_TRUE(result.is_valid());
}

TEST(GenericImageTest, WithMultipleEnvVars) {
  auto result = GenericImage("app", "latest")
                    .with_env_var("ENV", "production")
                    .with_env_var("DEBUG", "false")
                    .with_env_var("PORT", "8080");
  EXPECT_TRUE(result.is_valid());
}

TEST(GenericImageTest, WithHostname) {
  auto result = GenericImage("app", "latest").with_hostname("myapp.local");
  EXPECT_TRUE(result.is_valid());
}

TEST(GenericImageTest, WithMappedPort) {
  auto result = GenericImage("redis", "latest").with_mapped_port(6379, ContainerPort::Tcp(6379));
  EXPECT_TRUE(result.is_valid());
}

TEST(GenericImageTest, WithMappedPortDifferent) {
  auto result = GenericImage("nginx", "latest").with_mapped_port(8080, ContainerPort::Tcp(80));
  EXPECT_TRUE(result.is_valid());
}

TEST(GenericImageTest, WithPrivilegedTrue) {
  auto result = GenericImage("docker", "dind").with_privileged(true);
  EXPECT_TRUE(result.is_valid());
}

TEST(GenericImageTest, WithPrivilegedFalse) {
  auto result = GenericImage("alpine", "latest").with_privileged(false);
  EXPECT_TRUE(result.is_valid());
}

TEST(GenericImageTest, WithCapAdd) {
  auto result = GenericImage("app", "latest").with_cap_add("NET_ADMIN");
  EXPECT_TRUE(result.is_valid());
}

TEST(GenericImageTest, WithMultipleCapAdd) {
  auto result = GenericImage("app", "latest")
                    .with_cap_add("NET_ADMIN")
                    .with_cap_add("SYS_TIME")
                    .with_cap_add("SYS_PTRACE");
  EXPECT_TRUE(result.is_valid());
}

TEST(GenericImageTest, WithCapDrop) {
  auto result = GenericImage("app", "latest").with_cap_drop("NET_RAW");
  EXPECT_TRUE(result.is_valid());
}

TEST(GenericImageTest, WithCgroupnsModePrivate) {
  auto result = GenericImage("app", "latest").with_cgroupns_mode(CgroupnsMode::Private());
  EXPECT_TRUE(result.is_valid());
}

TEST(GenericImageTest, WithCgroupnsModeHost) {
  auto result = GenericImage("app", "latest").with_cgroupns_mode(CgroupnsMode::Host());
  EXPECT_TRUE(result.is_valid());
}

TEST(GenericImageTest, WithUsernsMode) {
  auto result = GenericImage("app", "latest").with_userns_mode("host");
  EXPECT_TRUE(result.is_valid());
}

TEST(GenericImageTest, WithShmSize) {
  auto result = GenericImage("postgres", "latest").with_shm_size(64 * 1024 * 1024); // 64 MB
  EXPECT_TRUE(result.is_valid());
}

TEST(GenericImageTest, WithShmSizeZero) {
  auto result = GenericImage("app", "latest").with_shm_size(0);
  EXPECT_TRUE(result.is_valid());
}

TEST(GenericImageTest, WithStartupTimeout) {
  auto result = GenericImage("app", "latest").with_startup_timeout(std::chrono::seconds(30));
  EXPECT_TRUE(result.is_valid());
}

TEST(GenericImageTest, WithStartupTimeoutMillis) {
  auto result = GenericImage("app", "latest").with_startup_timeout(std::chrono::milliseconds(5000));
  EXPECT_TRUE(result.is_valid());
}

TEST(GenericImageTest, WithWorkingDir) {
  auto result = GenericImage("app", "latest").with_working_dir("/app");
  EXPECT_TRUE(result.is_valid());
}

TEST(GenericImageTest, WithUser) {
  auto result = GenericImage("postgres", "latest").with_user("postgres");
  EXPECT_TRUE(result.is_valid());
}

TEST(GenericImageTest, WithUserUid) {
  auto result = GenericImage("app", "latest").with_user("1000");
  EXPECT_TRUE(result.is_valid());
}

TEST(GenericImageTest, WithUserUidGid) {
  auto result = GenericImage("app", "latest").with_user("1000:1000");
  EXPECT_TRUE(result.is_valid());
}

TEST(GenericImageTest, WithReadonlyRootfsTrue) {
  auto result = GenericImage("app", "latest").with_readonly_rootfs(true);
  EXPECT_TRUE(result.is_valid());
}

TEST(GenericImageTest, WithReadonlyRootfsFalse) {
  auto result = GenericImage("app", "latest").with_readonly_rootfs(false);
  EXPECT_TRUE(result.is_valid());
}

TEST(GenericImageTest, WithSecurityOpt) {
  auto result = GenericImage("app", "latest").with_security_opt("no-new-privileges");
  EXPECT_TRUE(result.is_valid());
}

TEST(GenericImageTest, WithSecurityOptApparmor) {
  auto result = GenericImage("app", "latest").with_security_opt("apparmor=unconfined");
  EXPECT_TRUE(result.is_valid());
}

TEST(GenericImageTest, WithReadyConditions) {
  std::vector<WaitFor> conditions = {WaitFor::message_on_stdout("Ready"), WaitFor::healthcheck()};
  auto result = GenericImage("app", "latest").with_ready_conditions(conditions);
  EXPECT_TRUE(result.is_valid());
}

TEST(GenericImageTest, WithReadyConditionsEmpty) {
  auto result = GenericImage("app", "latest").with_ready_conditions({});
  EXPECT_TRUE(result.is_valid());
}

TEST(GenericImageTest, WithCopyToFile) {
  TempFile source("config content");
  auto result = GenericImage("app", "latest")
                    .with_copy_to("/app/config.txt", CopyDataSource::File(source.path()));
  EXPECT_TRUE(result.is_valid());
}

TEST(GenericImageTest, WithCopyToData) {
  std::vector<std::uint8_t> data = {0x48, 0x65, 0x6C, 0x6C, 0x6F};
  auto result =
      GenericImage("app", "latest").with_copy_to("/app/data.bin", CopyDataSource::Data(data));
  EXPECT_TRUE(result.is_valid());
}

TEST(GenericImageTest, WithUlimit) {
  auto result = GenericImage("app", "latest").with_ulimit("nofile", 1024, std::nullopt);
  EXPECT_TRUE(result.is_valid());
}

TEST(GenericImageTest, WithUlimitHardLimit) {
  auto result = GenericImage("app", "latest").with_ulimit("nofile", 1024, 2048);
  EXPECT_TRUE(result.is_valid());
}

TEST(GenericImageTest, WithHealthCheck) {
  auto result =
      GenericImage("app", "latest")
          .with_health_check(Healthcheck::cmd_shell("curl -f http://localhost/ || exit 1"));
  EXPECT_TRUE(result.is_valid());
}

TEST(GenericImageTest, WithHealthCheckCmd) {
  auto result = GenericImage("app", "latest")
                    .with_health_check(Healthcheck::cmd({"curl", "-f", "http://localhost/"}));
  EXPECT_TRUE(result.is_valid());
}

TEST(GenericImageTest, WithHealthCheckNone) {
  auto result = GenericImage("app", "latest").with_health_check(Healthcheck::none());
  EXPECT_TRUE(result.is_valid());
}

TEST(GenericImageTest, WithHealthCheckWithInterval) {
  auto result =
      GenericImage("app", "latest")
          .with_health_check(
              Healthcheck::cmd_shell("test -f /ready").with_interval(std::chrono::seconds(5)));
  EXPECT_TRUE(result.is_valid());
}

// ====================
// Complex Fluent Chain Tests
// ====================

TEST(GenericImageTest, ComplexFluentChainIImage) {
  auto result = GenericImage("nginx", "latest")
                    .with_exposed_port(ContainerPort::Tcp(80))
                    .with_exposed_port(ContainerPort::Tcp(443))
                    .with_entrypoint("/docker-entrypoint.sh")
                    .with_wait_for(WaitFor::message_on_stdout("ready"));
  EXPECT_TRUE(result.is_valid());
}

TEST(GenericImageTest, ComplexFluentChainIImageExt) {
  auto result = GenericImage("redis", "7.2.4")
                    .with_env_var("REDIS_PASSWORD", "secret")
                    .with_env_var("REDIS_PORT", "6379")
                    .with_label("app", "redis")
                    .with_hostname("redis.local")
                    .with_working_dir("/data");
  EXPECT_TRUE(result.is_valid());
}

TEST(GenericImageTest, ComplexFluentChainMixed) {
  TempFile config("server { listen 80; }");
  auto result = GenericImage("nginx", "latest")
                    .with_exposed_port(ContainerPort::Tcp(80))
                    .with_wait_for(WaitFor::message_on_stdout("ready"))
                    .with_env_var("NGINX_HOST", "example.com")
                    .with_copy_to("/etc/nginx/nginx.conf", CopyDataSource::File(config.path()))
                    .with_user("nginx")
                    .with_readonly_rootfs(false);
  EXPECT_TRUE(result.is_valid());
}

// ====================
// Move Semantics Tests
// ====================

TEST(GenericImageTest, MoveConstructor) {
  GenericImage image1("redis", "latest");
  GenericImage image2(std::move(image1));
  EXPECT_TRUE(image2.is_valid());
}

TEST(GenericImageTest, MoveAssignment) {
  GenericImage image1("redis", "latest");
  GenericImage image2("nginx", "latest");
  image2 = std::move(image1);
  EXPECT_TRUE(image2.is_valid());
}

// ====================
// Edge Cases Tests
// ====================

TEST(GenericImageTest, VeryLongImageName) {
  std::string long_name(500, 'a');
  GenericImage image(long_name, "latest");
  EXPECT_TRUE(image.is_valid());
}

TEST(GenericImageTest, SpecialCharactersInTag) {
  GenericImage image("myapp", "v1.0.0-alpha+build.123");
  EXPECT_TRUE(image.is_valid());
}

TEST(GenericImageTest, UnicodeInEnvVar) {
  auto result = GenericImage("app", "latest").with_env_var("MESSAGE", "Привет мир! 你好世界! 🚀");
  EXPECT_TRUE(result.is_valid());
}

TEST(GenericImageTest, VeryLargeEnvVarValue) {
  std::string large_value(10000, 'x');
  auto result = GenericImage("app", "latest").with_env_var("LARGE_VAR", large_value);
  EXPECT_TRUE(result.is_valid());
}

TEST(GenericImageTest, EmptyStringsInVariousMethods) {
  auto result = GenericImage("alpine", "latest")
                    .with_entrypoint("")
                    .with_env_var("", "")
                    .with_hostname("")
                    .with_working_dir("")
                    .with_user("");
  EXPECT_TRUE(result.is_valid());
}
