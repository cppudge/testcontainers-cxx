#include <filesystem>

#include <gtest/gtest.h>

#include <testcontainers/system/Path.hpp>

using namespace testcontainers;

// ====================
// Path Constructor Tests
// ====================

TEST(PathTest, ConstructorFromStdPath) {
  std::filesystem::path p = "/tmp/test.txt";
  Path path(p);
  EXPECT_TRUE(path.is_valid());
}

TEST(PathTest, ConstructorFromString) {
  std::filesystem::path p = "test.txt";
  Path path(p);
  EXPECT_TRUE(path.is_valid());
}

TEST(PathTest, ConstructorRelativePath) {
  std::filesystem::path p = "./relative/path.txt";
  Path path(p);
  EXPECT_TRUE(path.is_valid());
}

TEST(PathTest, ConstructorAbsolutePath) {
  std::filesystem::path p = "/absolute/path.txt";
  Path path(p);
  EXPECT_TRUE(path.is_valid());
}

// ====================
// Path to_string Tests
// ====================

TEST(PathTest, ToStringSimple) {
  std::filesystem::path p = "test.txt";
  Path path(p);
  auto str = path.to_string();
  EXPECT_TRUE(str.has_value());
  EXPECT_EQ(str.value(), p.string());
}

TEST(PathTest, ToStringAbsolute) {
  std::filesystem::path p = "/tmp/test.txt";
  Path path(p);
  auto str = path.to_string();
  EXPECT_TRUE(str.has_value());
  EXPECT_EQ(str.value(), p.string());
}

TEST(PathTest, ToStringRelative) {
  std::filesystem::path p = "./relative/path.txt";
  Path path(p);
  auto str = path.to_string();
  EXPECT_TRUE(str.has_value());
  EXPECT_EQ(str.value(), p.string());
}

TEST(PathTest, ToStringNestedPath) {
  std::filesystem::path p = "/usr/local/bin/app";
  Path path(p);
  auto str = path.to_string();
  EXPECT_TRUE(str.has_value());
  EXPECT_EQ(str.value(), p.string());
}

// ====================
// Path Move Semantics Tests
// ====================

TEST(PathTest, MoveConstructor) {
  Path path1(std::filesystem::path("/tmp/test.txt"));
  Path path2(std::move(path1));
  EXPECT_TRUE(path2.is_valid());
}

TEST(PathTest, MoveAssignment) {
  Path path1(std::filesystem::path("/tmp/test.txt"));
  Path path2(std::filesystem::path("/other.txt"));
  path2 = std::move(path1);
  EXPECT_TRUE(path2.is_valid());
}

// ====================
// Path Edge Cases Tests
// ====================

TEST(PathTest, EmptyPath) {
  std::filesystem::path p;
  Path path(p);
  EXPECT_TRUE(path.is_valid());
}

TEST(PathTest, VeryLongPath) {
  std::string long_part(200, 'a');
  std::filesystem::path p = "/" + long_part + "/" + long_part + "/file.txt";
  Path path(p);
  EXPECT_TRUE(path.is_valid());
  auto str = path.to_string();
  EXPECT_TRUE(str.has_value());
  EXPECT_EQ(str.value(), p.string());
}

TEST(PathTest, PathWithSpaces) {
  std::filesystem::path p = "/path with spaces/file name.txt";
  Path path(p);
  EXPECT_TRUE(path.is_valid());
  auto str = path.to_string();
  EXPECT_TRUE(str.has_value());
  EXPECT_EQ(str.value(), p.string());
}

TEST(PathTest, PathWithSpecialChars) {
  std::filesystem::path p = "/path/file-name_123.txt";
  Path path(p);
  EXPECT_TRUE(path.is_valid());
  auto str = path.to_string();
  EXPECT_TRUE(str.has_value());
  EXPECT_EQ(str.value(), p.string());
}

// ====================
// Path Special Characters Tests
// ====================

TEST(PathTest, PathWithDots) {
  std::filesystem::path p = "/path/to/../file.txt";
  Path path(p);
  EXPECT_TRUE(path.is_valid());
  auto str = path.to_string();
  EXPECT_TRUE(str.has_value());
  EXPECT_EQ(str.value(), p.string());
}

TEST(PathTest, PathWithDashes) {
  std::filesystem::path p = "/my-path/my-file-name.txt";
  Path path(p);
  EXPECT_TRUE(path.is_valid());
  auto str = path.to_string();
  EXPECT_TRUE(str.has_value());
  EXPECT_EQ(str.value(), p.string());
}

TEST(PathTest, PathWithUnderscores) {
  std::filesystem::path p = "/my_path/my_file_name.txt";
  Path path(p);
  EXPECT_TRUE(path.is_valid());
  auto str = path.to_string();
  EXPECT_TRUE(str.has_value());
  EXPECT_EQ(str.value(), p.string());
}

TEST(PathTest, PathWithNumbers) {
  std::filesystem::path p = "/path123/file456.txt";
  Path path(p);
  EXPECT_TRUE(path.is_valid());
  auto str = path.to_string();
  EXPECT_TRUE(str.has_value());
  EXPECT_EQ(str.value(), p.string());
}

TEST(PathTest, PathWithMultipleExtensions) {
  std::filesystem::path p = "/path/file.tar.gz";
  Path path(p);
  EXPECT_TRUE(path.is_valid());
  auto str = path.to_string();
  EXPECT_TRUE(str.has_value());
  EXPECT_EQ(str.value(), p.string());
}

TEST(PathTest, PathWithParentheses) {
  std::filesystem::path p = "/path/file(1).txt";
  Path path(p);
  EXPECT_TRUE(path.is_valid());
  auto str = path.to_string();
  EXPECT_TRUE(str.has_value());
  EXPECT_EQ(str.value(), p.string());
}

TEST(PathTest, PathWithBrackets) {
  std::filesystem::path p = "/path/file[123].txt";
  Path path(p);
  EXPECT_TRUE(path.is_valid());
  auto str = path.to_string();
  EXPECT_TRUE(str.has_value());
  EXPECT_EQ(str.value(), p.string());
}

TEST(PathTest, PathWithPlus) {
  std::filesystem::path p = "/path/file+version.txt";
  Path path(p);
  EXPECT_TRUE(path.is_valid());
  auto str = path.to_string();
  EXPECT_TRUE(str.has_value());
  EXPECT_EQ(str.value(), p.string());
}

TEST(PathTest, PathWithEquals) {
  std::filesystem::path p = "/path/key=value.txt";
  Path path(p);
  EXPECT_TRUE(path.is_valid());
  auto str = path.to_string();
  EXPECT_TRUE(str.has_value());
  EXPECT_EQ(str.value(), p.string());
}

TEST(PathTest, PathWithAt) {
  std::filesystem::path p = "/path/file@2x.png";
  Path path(p);
  EXPECT_TRUE(path.is_valid());
  auto str = path.to_string();
  EXPECT_TRUE(str.has_value());
  EXPECT_EQ(str.value(), p.string());
}

TEST(PathTest, PathWithTilde) {
  std::filesystem::path p = "/path/~backup.txt";
  Path path(p);
  EXPECT_TRUE(path.is_valid());
  auto str = path.to_string();
  EXPECT_TRUE(str.has_value());
  EXPECT_EQ(str.value(), p.string());
}

TEST(PathTest, PathWithPercent) {
  std::filesystem::path p = "/path/file%20name.txt";
  Path path(p);
  EXPECT_TRUE(path.is_valid());
  auto str = path.to_string();
  EXPECT_TRUE(str.has_value());
  EXPECT_EQ(str.value(), p.string());
}

TEST(PathTest, PathWithAmpersand) {
  std::filesystem::path p = "/path/file&data.txt";
  Path path(p);
  EXPECT_TRUE(path.is_valid());
  auto str = path.to_string();
  EXPECT_TRUE(str.has_value());
  EXPECT_EQ(str.value(), p.string());
}

TEST(PathTest, PathWithDollar) {
  std::filesystem::path p = "/path/$variable.txt";
  Path path(p);
  EXPECT_TRUE(path.is_valid());
  auto str = path.to_string();
  EXPECT_TRUE(str.has_value());
  EXPECT_EQ(str.value(), p.string());
}

TEST(PathTest, PathWithHash) {
  std::filesystem::path p = "/path/file#tag.txt";
  Path path(p);
  EXPECT_TRUE(path.is_valid());
  auto str = path.to_string();
  EXPECT_TRUE(str.has_value());
  EXPECT_EQ(str.value(), p.string());
}

TEST(PathTest, PathWithComma) {
  std::filesystem::path p = "/path/file,data.csv";
  Path path(p);
  EXPECT_TRUE(path.is_valid());
  auto str = path.to_string();
  EXPECT_TRUE(str.has_value());
  EXPECT_EQ(str.value(), p.string());
}

TEST(PathTest, PathWithSemicolon) {
  std::filesystem::path p = "/path/file;backup.txt";
  Path path(p);
  EXPECT_TRUE(path.is_valid());
  auto str = path.to_string();
  EXPECT_TRUE(str.has_value());
  EXPECT_EQ(str.value(), p.string());
}

TEST(PathTest, PathWithExclamation) {
  std::filesystem::path p = "/path/file!important.txt";
  Path path(p);
  EXPECT_TRUE(path.is_valid());
  auto str = path.to_string();
  EXPECT_TRUE(str.has_value());
  EXPECT_EQ(str.value(), p.string());
}

TEST(PathTest, PathWithApostrophe) {
  std::filesystem::path p = "/path/file's.txt";
  Path path(p);
  EXPECT_TRUE(path.is_valid());
  auto str = path.to_string();
  EXPECT_TRUE(str.has_value());
  EXPECT_EQ(str.value(), p.string());
}

TEST(PathTest, PathWithQuotes) {
  std::filesystem::path p = "/path/file\"quoted\".txt";
  Path path(p);
  EXPECT_TRUE(path.is_valid());
  auto str = path.to_string();
  EXPECT_TRUE(str.has_value());
  EXPECT_EQ(str.value(), p.string());
}

TEST(PathTest, PathWithBackslash) {
  // На Windows это разделитель пути, на Unix - обычный символ
  std::filesystem::path p = "/path/file\\name.txt";
  Path path(p);
  EXPECT_TRUE(path.is_valid());
  auto str = path.to_string();
  EXPECT_TRUE(str.has_value());
}

TEST(PathTest, PathWithMultipleSpecialChars) {
  std::filesystem::path p = "/path-to_file/my-file_v1.2.3+build[123]@2x.tar.gz";
  Path path(p);
  EXPECT_TRUE(path.is_valid());
  auto str = path.to_string();
  EXPECT_TRUE(str.has_value());
  EXPECT_EQ(str.value(), p.string());
}

// ====================
// Path Unicode Tests
// ====================

TEST(PathTest, PathWithCyrillic) {
  std::filesystem::path p = "/путь/файл.txt";
  Path path(p);
  EXPECT_TRUE(path.is_valid());
  auto str = path.to_string();
  EXPECT_TRUE(str.has_value());
}

TEST(PathTest, PathWithChinese) {
  std::filesystem::path p = "/路径/文件.txt";
  Path path(p);
  EXPECT_TRUE(path.is_valid());
  auto str = path.to_string();
  EXPECT_TRUE(str.has_value());
}

TEST(PathTest, PathWithEmoji) {
  std::filesystem::path p = "/path/file🚀test.txt";
  Path path(p);
  EXPECT_TRUE(path.is_valid());
  auto str = path.to_string();
  EXPECT_TRUE(str.has_value());
}

TEST(PathTest, PathWithMixedUnicode) {
  std::filesystem::path p = "/путь/path/文件/file_Привет_世界.txt";
  Path path(p);
  EXPECT_TRUE(path.is_valid());
  auto str = path.to_string();
  EXPECT_TRUE(str.has_value());
}

// ====================
// Path Normalization Tests
// ====================

TEST(PathTest, PathWithCurrentDir) {
  std::filesystem::path p = "./file.txt";
  Path path(p);
  EXPECT_TRUE(path.is_valid());
  auto str = path.to_string();
  EXPECT_TRUE(str.has_value());
  EXPECT_EQ(str.value(), p.string());
}

TEST(PathTest, PathWithParentDir) {
  std::filesystem::path p = "../file.txt";
  Path path(p);
  EXPECT_TRUE(path.is_valid());
  auto str = path.to_string();
  EXPECT_TRUE(str.has_value());
  EXPECT_EQ(str.value(), p.string());
}

TEST(PathTest, PathWithMultipleDots) {
  std::filesystem::path p = "/path/./to/../file.txt";
  Path path(p);
  EXPECT_TRUE(path.is_valid());
  auto str = path.to_string();
  EXPECT_TRUE(str.has_value());
  EXPECT_EQ(str.value(), p.string());
}

TEST(PathTest, PathWithTrailingSlash) {
  std::filesystem::path p = "/path/to/dir/";
  Path path(p);
  EXPECT_TRUE(path.is_valid());
  auto str = path.to_string();
  EXPECT_TRUE(str.has_value());
}

TEST(PathTest, PathWithDoubleSlash) {
  std::filesystem::path p = "/path//to//file.txt";
  Path path(p);
  EXPECT_TRUE(path.is_valid());
  auto str = path.to_string();
  EXPECT_TRUE(str.has_value());
}

