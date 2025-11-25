#include <filesystem>

#include <gtest/gtest.h>

#include <testcontainers/testcontainers.hpp>

#include "testutils/TempFile.hpp"

using namespace testcontainers;
using namespace testcontainers::test_utils;
namespace fs = std::filesystem;

// ====================
// Constructor Tests
// ====================

TEST(GenericBuildableImageTest, ConstructorWithValidNameAndTag) {
  GenericBuildableImage image("myapp", "latest");
  EXPECT_TRUE(image.is_valid());
}

TEST(GenericBuildableImageTest, ConstructorWithEmptyName) {
  GenericBuildableImage image("", "latest");
  EXPECT_TRUE(image.is_valid());
}

TEST(GenericBuildableImageTest, ConstructorWithEmptyTag) {
  GenericBuildableImage image("myapp", "");
  EXPECT_TRUE(image.is_valid());
}

// ====================
// with_dockerfile Tests
// ====================

TEST(GenericBuildableImageTest, WithDockerfileValidPath) {
  TempFile dockerfile("FROM alpine:latest");

  GenericBuildableImage image("myapp", "latest");
  auto result = image.with_dockerfile(dockerfile.path());
  EXPECT_TRUE(result.is_valid());
}

TEST(GenericBuildableImageTest, WithDockerfileNonExistentPath) {
  // Путь не существует, но на уровне C++ мы просто передаём его в Rust
  // Ошибка возникнет только при вызове build()
  GenericBuildableImage image("myapp", "latest");
  auto result = image.with_dockerfile(fs::path("/nonexistent/Dockerfile"));
  EXPECT_TRUE(result.is_valid());
}

TEST(GenericBuildableImageTest, WithDockerfileFluentChain) {
  TempFile dockerfile("FROM alpine:latest");

  auto result = GenericBuildableImage("myapp", "latest").with_dockerfile(dockerfile.path());
  EXPECT_TRUE(result.is_valid());
}

// ====================
// with_dockerfile_string Tests
// ====================

TEST(GenericBuildableImageTest, WithDockerfileStringValid) {
  GenericBuildableImage image("myapp", "latest");
  auto result = image.with_dockerfile_string("FROM alpine:latest\nRUN echo 'Hello'");
  EXPECT_TRUE(result.is_valid());
}

TEST(GenericBuildableImageTest, WithDockerfileStringEmpty) {
  GenericBuildableImage image("myapp", "latest");
  auto result = image.with_dockerfile_string("");
  EXPECT_TRUE(result.is_valid());
}

TEST(GenericBuildableImageTest, WithDockerfileStringMultiline) {
  std::string dockerfile = "FROM ubuntu:22.04\n"
                           "RUN apt-get update && apt-get install -y curl\n"
                           "WORKDIR /app\n"
                           "COPY . .\n"
                           "CMD [\"./run.sh\"]";

  auto result = GenericBuildableImage("myapp", "latest").with_dockerfile_string(dockerfile);
  EXPECT_TRUE(result.is_valid());
}

// ====================
// with_file Tests
// ====================

TEST(GenericBuildableImageTest, WithFileValidSourceAndTarget) {
  TempFile source("test content");

  GenericBuildableImage image("myapp", "latest");
  auto result = image.with_file(source.path(), "/app/config.txt");
  EXPECT_TRUE(result.is_valid());
}

TEST(GenericBuildableImageTest, WithFileNonExistentSource) {
  // Файл не существует, но на уровне C++ мы просто передаём путь
  GenericBuildableImage image("myapp", "latest");
  auto result = image.with_file(fs::path("/nonexistent/file.txt"), "/app/file.txt");
  EXPECT_TRUE(result.is_valid());
}

TEST(GenericBuildableImageTest, WithFileMultipleFiles) {
  TempFile file1("content1");
  TempFile file2("content2");

  auto result = GenericBuildableImage("myapp", "latest")
                    .with_file(file1.path(), "/app/file1.txt")
                    .with_file(file2.path(), "/app/file2.txt");
  EXPECT_TRUE(result.is_valid());
}

TEST(GenericBuildableImageTest, WithFileEmptyTarget) {
  TempFile source("test content");

  GenericBuildableImage image("myapp", "latest");
  auto result = image.with_file(source.path(), "");
  EXPECT_TRUE(result.is_valid());
}

// ====================
// with_data Tests
// ====================

TEST(GenericBuildableImageTest, WithDataValidDataAndTarget) {
  std::vector<std::uint8_t> data = {0x48, 0x65, 0x6C, 0x6C, 0x6F}; // "Hello"

  GenericBuildableImage image("myapp", "latest");
  auto result = image.with_data(data, "/app/data.bin");
  EXPECT_TRUE(result.is_valid());
}

TEST(GenericBuildableImageTest, WithDataEmptyData) {
  std::vector<std::uint8_t> data;

  GenericBuildableImage image("myapp", "latest");
  auto result = image.with_data(data, "/app/empty.bin");
  EXPECT_TRUE(result.is_valid());
}

TEST(GenericBuildableImageTest, WithDataLargeData) {
  std::vector<std::uint8_t> data(1024 * 1024, 0xFF); // 1 MB

  GenericBuildableImage image("myapp", "latest");
  auto result = image.with_data(data, "/app/large.bin");
  EXPECT_TRUE(result.is_valid());
}

TEST(GenericBuildableImageTest, WithDataMultipleData) {
  std::vector<std::uint8_t> data1 = {0x01, 0x02, 0x03};
  std::vector<std::uint8_t> data2 = {0x04, 0x05, 0x06};

  auto result = GenericBuildableImage("myapp", "latest")
                    .with_data(data1, "/app/data1.bin")
                    .with_data(data2, "/app/data2.bin");
  EXPECT_TRUE(result.is_valid());
}

TEST(GenericBuildableImageTest, WithDataEmptyTarget) {
  std::vector<std::uint8_t> data = {0x48, 0x65, 0x6C, 0x6C, 0x6F};

  GenericBuildableImage image("myapp", "latest");
  auto result = image.with_data(data, "");
  EXPECT_TRUE(result.is_valid());
}

// ====================
// Complex Fluent Chain Tests
// ====================

TEST(GenericBuildableImageTest, FluentChainAllMethods) {
  TempFile dockerfile("FROM alpine:latest");
  TempFile file("config content");
  std::vector<std::uint8_t> data = {0x01, 0x02, 0x03};

  auto result = GenericBuildableImage("myapp", "v1.0")
                    .with_dockerfile(dockerfile.path())
                    .with_file(file.path(), "/app/config.txt")
                    .with_data(data, "/app/data.bin");
  EXPECT_TRUE(result.is_valid());
}

TEST(GenericBuildableImageTest, FluentChainWithDockerfileString) {
  std::vector<std::uint8_t> data = {0xFF, 0xFE};

  auto result = GenericBuildableImage("webapp", "dev")
                    .with_dockerfile_string("FROM nginx:latest")
                    .with_data(data, "/usr/share/nginx/html/index.html");
  EXPECT_TRUE(result.is_valid());
}

// ====================
// build() Tests
// ====================

TEST(GenericBuildableImageTest, BuildWithInvalidDockerfileContentFails) {
  // Даже с валидным синтаксисом C++, но без Docker daemon
  EXPECT_THROW(
      {
        auto result = GenericBuildableImage("invalid", "test")
                          .with_dockerfile_string("INVALID DOCKERFILE SYNTAX")
                          .build();
      },
      Error);
}

// ====================
// Move Semantics Tests
// ====================

TEST(GenericBuildableImageTest, MoveConstructor) {
  GenericBuildableImage image1("myapp", "latest");
  GenericBuildableImage image2(std::move(image1));
  EXPECT_TRUE(image2.is_valid());
}

TEST(GenericBuildableImageTest, MoveAssignment) {
  GenericBuildableImage image1("myapp", "latest");
  GenericBuildableImage image2("other", "v1");
  image2 = std::move(image1);
  EXPECT_TRUE(image2.is_valid());
}

// ====================
// Edge Cases Tests
// ====================

TEST(GenericBuildableImageTest, VeryLongNameAndTag) {
  std::string long_name(1000, 'a');
  std::string long_tag(1000, 'b');

  GenericBuildableImage image(long_name, long_tag);
  EXPECT_TRUE(image.is_valid());
}

TEST(GenericBuildableImageTest, SpecialCharactersInNameAndTag) {
  GenericBuildableImage image("my-app_123", "v1.0.0-alpha+build.123");
  EXPECT_TRUE(image.is_valid());
}

TEST(GenericBuildableImageTest, UnicodeInDockerfileString) {
  std::string dockerfile = "FROM alpine:latest\nRUN echo 'Привет мир! 你好世界! 🚀'";

  auto result = GenericBuildableImage("unicode-test", "latest").with_dockerfile_string(dockerfile);
  EXPECT_TRUE(result.is_valid());
}

TEST(GenericBuildableImageTest, BinaryDataWithNullBytes) {
  std::vector<std::uint8_t> data = {0x00, 0xFF, 0x00, 0x7F, 0x80};

  auto result = GenericBuildableImage("binary-test", "latest").with_data(data, "/app/binary.dat");
  EXPECT_TRUE(result.is_valid());
}
