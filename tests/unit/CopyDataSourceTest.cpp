#include <filesystem>
#include <vector>

#include <gtest/gtest.h>

#include <testcontainers/core/CopyDataSource.hpp>

#include "testutils/TempFile.hpp"

using namespace testcontainers;
using testcontainers::test_utils::TempFile;

// ====================
// CopyDataSource::File Tests
// ====================

TEST(CopyDataSourceTest, FileFromTempFile) {
  TempFile temp("test content");
  auto source = CopyDataSource::File(temp.path());
  EXPECT_TRUE(source.is_valid());
}

TEST(CopyDataSourceTest, FileFromString) {
  auto source = CopyDataSource::File("/tmp/test.txt");
  EXPECT_TRUE(source.is_valid());
}

TEST(CopyDataSourceTest, FileFromStdPath) {
  std::filesystem::path p = "/etc/config.txt";
  auto source = CopyDataSource::File(p);
  EXPECT_TRUE(source.is_valid());
}

TEST(CopyDataSourceTest, FileNonExistentPath) {
  auto source = CopyDataSource::File("/nonexistent/path/file.txt");
  EXPECT_TRUE(source.is_valid());
}

TEST(CopyDataSourceTest, FileRelativePath) {
  auto source = CopyDataSource::File("./relative/path.txt");
  EXPECT_TRUE(source.is_valid());
}

// ====================
// CopyDataSource::Data Tests
// ====================

TEST(CopyDataSourceTest, DataBasic) {
  std::vector<std::uint8_t> data = {0x48, 0x65, 0x6C, 0x6C, 0x6F}; // "Hello"
  auto source = CopyDataSource::Data(data);
  EXPECT_TRUE(source.is_valid());
}

TEST(CopyDataSourceTest, DataEmpty) {
  std::vector<std::uint8_t> data;
  auto source = CopyDataSource::Data(data);
  EXPECT_TRUE(source.is_valid());
}

TEST(CopyDataSourceTest, DataLarge) {
  std::vector<std::uint8_t> data(1024 * 1024, 0xFF); // 1 MB
  auto source = CopyDataSource::Data(data);
  EXPECT_TRUE(source.is_valid());
}

TEST(CopyDataSourceTest, DataBinary) {
  std::vector<std::uint8_t> data = {0x00, 0xFF, 0x00, 0x7F, 0x80, 0xAA, 0x55};
  auto source = CopyDataSource::Data(data);
  EXPECT_TRUE(source.is_valid());
}

TEST(CopyDataSourceTest, DataWithNullBytes) {
  std::vector<std::uint8_t> data = {0x01, 0x00, 0x02, 0x00, 0x03};
  auto source = CopyDataSource::Data(data);
  EXPECT_TRUE(source.is_valid());
}

// ====================
// CopyDataSource Move Semantics Tests
// ====================

TEST(CopyDataSourceTest, MoveConstructor) {
  std::vector<std::uint8_t> data = {0x01, 0x02, 0x03};
  auto source1 = CopyDataSource::Data(data);
  auto source2 = std::move(source1);
  EXPECT_TRUE(source2.is_valid());
}

TEST(CopyDataSourceTest, MoveAssignment) {
  std::vector<std::uint8_t> data1 = {0x01, 0x02};
  std::vector<std::uint8_t> data2 = {0x03, 0x04};
  auto source1 = CopyDataSource::Data(data1);
  auto source2 = CopyDataSource::Data(data2);
  source2 = std::move(source1);
  EXPECT_TRUE(source2.is_valid());
}

// ====================
// CopyDataSource Edge Cases Tests
// ====================

TEST(CopyDataSourceTest, VeryLargePath) {
  std::string long_part(300, 'a');
  std::filesystem::path p = "/" + long_part + "/" + long_part + "/file.txt";
  auto source = CopyDataSource::File(p);
  EXPECT_TRUE(source.is_valid());
}

TEST(CopyDataSourceTest, PathWithSpaces) {
  auto source = CopyDataSource::File("/path with spaces/file name.txt");
  EXPECT_TRUE(source.is_valid());
}

TEST(CopyDataSourceTest, PathWithUnicode) {
  auto source = CopyDataSource::File("/путь/файл.txt");
  EXPECT_TRUE(source.is_valid());
}

TEST(CopyDataSourceTest, DataSingleByte) {
  std::vector<std::uint8_t> data = {0x42};
  auto source = CopyDataSource::Data(data);
  EXPECT_TRUE(source.is_valid());
}

TEST(CopyDataSourceTest, DataAllZeros) {
  std::vector<std::uint8_t> data(100, 0x00);
  auto source = CopyDataSource::Data(data);
  EXPECT_TRUE(source.is_valid());
}

TEST(CopyDataSourceTest, DataAllOnes) {
  std::vector<std::uint8_t> data(100, 0xFF);
  auto source = CopyDataSource::Data(data);
  EXPECT_TRUE(source.is_valid());
}

TEST(CopyDataSourceTest, DataVeryLarge) {
  std::vector<std::uint8_t> data(10 * 1024 * 1024, 0xAA); // 10 MB
  auto source = CopyDataSource::Data(data);
  EXPECT_TRUE(source.is_valid());
}

