#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <testcontainers/testcontainers.hpp>

#include "testutils/TempFile.hpp"

using namespace testcontainers;
using namespace testcontainers::test_utils;

using ::testing::HasSubstr;

// ====================
// Basic Build Tests
// ====================

TEST(GenericBuildableImageIntegrationTest, BuildWithDockerfileString) {
  std::string dockerfile = R"(
FROM alpine:latest
RUN echo "Hello from build" > /test.txt
CMD ["cat", "/test.txt"]
)";

  auto image
      = GenericBuildableImage("testcontainers_integration_test_container", "dockerfile_string")
            .with_dockerfile_string(dockerfile)
            .build();

  EXPECT_TRUE(image.is_valid());
  EXPECT_EQ(image.name(), "testcontainers_integration_test_container");
  EXPECT_EQ(image.tag(), "dockerfile_string");

  // Запускаем контейнер и проверяем вывод
  auto container = image.start();
  EXPECT_THAT(container.stdout_to_string(), HasSubstr("Hello from build"));
}

TEST(GenericBuildableImageIntegrationTest, BuildWithDockerfileFile) {
  std::string dockerfile_content = R"(
FROM alpine:latest
RUN echo "Built from file" > /output.txt
CMD ["cat", "/output.txt"]
)";

  TempFile dockerfile(dockerfile_content);

  auto image = GenericBuildableImage("testcontainers_integration_test_container", "dockerfile_file")
                   .with_dockerfile(dockerfile.path())
                   .build();

  EXPECT_TRUE(image.is_valid());

  auto container = image.start();
  EXPECT_THAT(container.stdout_to_string(), HasSubstr("Built from file"));
}

// ====================
// Build with Files Tests
// ====================

TEST(GenericBuildableImageIntegrationTest, BuildWithSingleFile) {
  TempFile config("server_port=8080\nserver_host=localhost\n");

  std::string dockerfile = R"(
FROM alpine:latest
COPY config.txt /app/config.txt
CMD ["cat", "/app/config.txt"]
)";

  auto image = GenericBuildableImage("testcontainers_integration_test_container", "single_file")
                   .with_dockerfile_string(dockerfile)
                   .with_file(config.path(), "config.txt")
                   .build();

  auto container = image.start();

  auto stdout_str = container.stdout_to_string();
  EXPECT_THAT(stdout_str, HasSubstr("server_port=8080"));
  EXPECT_THAT(stdout_str, HasSubstr("server_host=localhost"));
}

TEST(GenericBuildableImageIntegrationTest, BuildWithMultipleFiles) {
  TempFile file1("Content of file 1");
  TempFile file2("Content of file 2");

  std::string dockerfile = R"(
FROM alpine:latest
COPY file1.txt /app/file1.txt
COPY file2.txt /app/file2.txt
CMD ["sh", "-c", "cat /app/file1.txt && cat /app/file2.txt"]
)";

  auto image = GenericBuildableImage("testcontainers_integration_test_container", "multiple_files")
                   .with_dockerfile_string(dockerfile)
                   .with_file(file1.path(), "file1.txt")
                   .with_file(file2.path(), "file2.txt")
                   .build();

  auto container = image.start();

  auto stdout_str = container.stdout_to_string();
  EXPECT_THAT(stdout_str, HasSubstr("Content of file 1"));
  EXPECT_THAT(stdout_str, HasSubstr("Content of file 2"));
}

// ====================
// Build with Data Tests
// ====================

TEST(GenericBuildableImageIntegrationTest, BuildWithBinaryData) {
  std::vector<std::uint8_t> data = {0x48, 0x65, 0x6C, 0x6C, 0x6F}; // "Hello"

  std::string dockerfile = R"(
FROM alpine:latest
COPY data.bin /app/data.bin
CMD ["cat", "/app/data.bin"]
)";

  auto image = GenericBuildableImage("testcontainers_integration_test_container", "binary_data")
                   .with_dockerfile_string(dockerfile)
                   .with_data(data, "data.bin")
                   .build();

  auto container = image.start();

  EXPECT_EQ(container.stdout_to_vec(), data);
}

TEST(GenericBuildableImageIntegrationTest, BuildWithMultipleDataFiles) {
  std::vector<std::uint8_t> data1 = {0x41, 0x42, 0x43}; // "ABC"
  std::vector<std::uint8_t> data2 = {0x58, 0x59, 0x5A}; // "XYZ"

  std::string dockerfile = R"(
FROM alpine:latest
COPY data1.bin /app/data1.bin
COPY data2.bin /app/data2.bin
CMD ["sh", "-c", "cat /app/data1.bin && cat /app/data2.bin"]
)";

  auto image = GenericBuildableImage("testcontainers_integration_test_container", "multiple_data")
                   .with_dockerfile_string(dockerfile)
                   .with_data(data1, "data1.bin")
                   .with_data(data2, "data2.bin")
                   .build();

  auto container = image.start();

  std::vector<std::uint8_t> expected = {0x41, 0x42, 0x43, 0x58, 0x59, 0x5A};
  EXPECT_EQ(container.stdout_to_vec(), expected);
}

// ====================
// Complex Build Scenarios Tests
// ====================

TEST(GenericBuildableImageIntegrationTest, BuildWithMultiStageDockerfile) {
  std::string dockerfile = R"(
FROM alpine:latest AS builder
RUN echo "Build stage" > /build.txt

FROM alpine:latest
COPY --from=builder /build.txt /app/build.txt
RUN echo "Runtime stage" > /app/runtime.txt
CMD ["sh", "-c", "cat /app/build.txt && cat /app/runtime.txt"]
)";

  auto image = GenericBuildableImage("testcontainers_integration_test_container", "multi_stage")
                   .with_dockerfile_string(dockerfile)
                   .build();

  auto container = image.start();

  auto stdout_str = container.stdout_to_string();
  EXPECT_THAT(stdout_str, HasSubstr("Build stage"));
  EXPECT_THAT(stdout_str, HasSubstr("Runtime stage"));
}

TEST(GenericBuildableImageIntegrationTest, BuildWithEnvironmentVariables) {
  std::string dockerfile = R"(
FROM alpine:latest
ENV APP_NAME="TestApp"
ENV APP_VERSION="1.0.0"
CMD ["sh", "-c", "echo $APP_NAME $APP_VERSION"]
)";

  auto image = GenericBuildableImage("testcontainers_integration_test_container", "env_vars")
                   .with_dockerfile_string(dockerfile)
                   .build();

  auto container = image.start();

  EXPECT_THAT(container.stdout_to_string(), HasSubstr("TestApp 1.0.0"));
}

TEST(GenericBuildableImageIntegrationTest, BuildWithWorkdir) {
  std::string dockerfile = R"(
FROM alpine:latest
WORKDIR /app
RUN pwd > current_dir.txt
CMD ["cat", "current_dir.txt"]
)";

  auto image = GenericBuildableImage("testcontainers_integration_test_container", "workdir")
                   .with_dockerfile_string(dockerfile)
                   .build();

  auto container = image.start();

  EXPECT_THAT(container.stdout_to_string(), HasSubstr("/app"));
}

TEST(GenericBuildableImageIntegrationTest, BuildWithRunCommands) {
  std::string dockerfile = R"(
FROM alpine:latest
RUN echo "Step 1" > /log.txt
RUN echo "Step 2" >> /log.txt
RUN echo "Step 3" >> /log.txt
CMD ["cat", "/log.txt"]
)";

  auto image = GenericBuildableImage("testcontainers_integration_test_container", "run_commands")
                   .with_dockerfile_string(dockerfile)
                   .build();

  auto container = image.start();

  EXPECT_THAT(container.stdout_to_string(), HasSubstr("Step 1\nStep 2\nStep 3\n"));
}

// ====================
// File Permissions and Ownership Tests
// ====================

TEST(GenericBuildableImageIntegrationTest, BuildWithFilePermissions) {
  TempFile script("#!/bin/sh\necho 'Executable script'\n");

  std::string dockerfile = R"(
FROM alpine:latest
COPY script.sh /app/script.sh
RUN chmod +x /app/script.sh
CMD ["/bin/sh", "/app/script.sh"]
)";

  auto image = GenericBuildableImage("testcontainers_integration_test_container", "permissions")
                   .with_dockerfile_string(dockerfile)
                   .with_file(script.path(), "script.sh")
                   .build();

  auto container = image.start();

  EXPECT_THAT(container.stdout_to_string(), HasSubstr("Executable script"));
}

TEST(GenericBuildableImageIntegrationTest, BuildWithDirectoryCreation) {
  std::string dockerfile = R"(
FROM alpine:latest
RUN mkdir -p /app/data/logs
RUN touch /app/data/logs/test.log
RUN echo "Log entry" > /app/data/logs/test.log
CMD ["cat", "/app/data/logs/test.log"]
)";

  auto image = GenericBuildableImage("testcontainers_integration_test_container", "directories")
                   .with_dockerfile_string(dockerfile)
                   .build();

  auto container = image.start();

  EXPECT_THAT(container.stdout_to_string(), HasSubstr("Log entry"));
}

// ====================
// Package Installation Tests
// ====================

TEST(GenericBuildableImageIntegrationTest, BuildWithPackageInstallation) {
  std::string dockerfile = R"(
FROM alpine:latest
RUN apk add --no-cache curl
RUN curl --version > /curl_version.txt
CMD ["head", "-n", "1", "/curl_version.txt"]
)";

  auto image = GenericBuildableImage("testcontainers_integration_test_container", "with_curl")
                   .with_dockerfile_string(dockerfile)
                   .build();

  auto container = image.start();

  EXPECT_THAT(container.stdout_to_string(), HasSubstr("curl"));
}

// ====================
// Error Handling Tests
// ====================

TEST(GenericBuildableImageIntegrationTest, BuildWithInvalidDockerfile) {
  std::string invalid_dockerfile = R"(
INVALID_INSTRUCTION alpine:latest
RUN echo "This should fail"
)";

  EXPECT_THROW(
      {
        GenericBuildableImage("testcontainers_integration_test_container", "invalid")
            .with_dockerfile_string(invalid_dockerfile)
            .build();
      },
      Error);
}

TEST(GenericBuildableImageIntegrationTest, BuildWithNonExistentBaseImage) {
  std::string dockerfile = R"(
FROM nonexistent-image-12345:latest
RUN echo "This should fail"
)";

  EXPECT_THROW(
      {
        GenericBuildableImage("testcontainers_integration_test_container", "nonexistent")
            .with_dockerfile_string(dockerfile)
            .build();
      },
      Error);
}

// ====================
// Edge Cases Tests
// ====================

TEST(GenericBuildableImageIntegrationTest, BuildMinimalImage) {
  std::string dockerfile = R"(
FROM alpine:latest
CMD ["echo", "OK"]
)";

  auto image = GenericBuildableImage("testcontainers_integration_test_container", "minimal")
                   .with_dockerfile_string(dockerfile)
                   .build();

  auto container = image.start();

  EXPECT_THAT(container.stdout_to_string(), HasSubstr("OK"));
}

TEST(GenericBuildableImageIntegrationTest, BuildWithLongRunningCommand) {
  std::string dockerfile = R"(
FROM alpine:latest
RUN for i in $(seq 1 5); do echo "Step $i"; sleep 0.1; done
CMD ["echo", "Build completed"]
)";

  auto image = GenericBuildableImage("testcontainers_integration_test_container", "long_build")
                   .with_dockerfile_string(dockerfile)
                   .build();

  auto container = image.start();

  EXPECT_THAT(container.stdout_to_string(), HasSubstr("Build completed"));
}

TEST(GenericBuildableImageIntegrationTest, BuildWithUnicodeContent) {
  TempFile unicode_file("Привет мир! 你好世界! 🚀");

  std::string dockerfile = R"(
FROM alpine:latest
COPY unicode.txt /app/unicode.txt
CMD ["cat", "/app/unicode.txt"]
)";

  auto image = GenericBuildableImage("testcontainers_integration_test_container", "unicode")
                   .with_dockerfile_string(dockerfile)
                   .with_file(unicode_file.path(), "unicode.txt")
                   .build();

  auto container = image.start();

  EXPECT_THAT(container.stdout_to_string(), HasSubstr("Привет мир! 你好世界! 🚀"));
}

// ====================
// Rebuild and Caching Tests
// ====================

TEST(GenericBuildableImageIntegrationTest, RebuildSameImage) {
  std::string dockerfile = R"(
FROM alpine:latest
RUN echo "Rebuild test" > /test.txt
CMD ["cat", "/test.txt"]
)";

  // Первая сборка
  auto image1 = GenericBuildableImage("testcontainers_integration_test_container", "rebuild")
                    .with_dockerfile_string(dockerfile)
                    .build();

  // Вторая сборка с тем же содержимым (должна использовать кэш)
  auto image2 = GenericBuildableImage("testcontainers_integration_test_container", "rebuild")
                    .with_dockerfile_string(dockerfile)
                    .build();

  auto container1 = image1.start();
  auto container2 = image2.start();

  EXPECT_EQ(container1.stdout_to_string(), container2.stdout_to_string());
}
