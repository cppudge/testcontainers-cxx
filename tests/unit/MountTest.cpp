#include <gtest/gtest.h>

#include <testcontainers/core/Mount.hpp>

using namespace testcontainers;

// ====================
// Mount::BindMount Tests
// ====================

TEST(MountTest, BindMountBasic) {
  auto mount = Mount::BindMount("/host/path", "/container/path");
  EXPECT_TRUE(mount.is_valid());
}

TEST(MountTest, BindMountWithReadOnly) {
  auto mount = Mount::BindMount("/host/data", "/app/data").with_read_only();
  EXPECT_TRUE(mount.is_valid());
}

TEST(MountTest, BindMountWithReadWrite) {
  auto mount = Mount::BindMount("/host/data", "/app/data").with_read_write();
  EXPECT_TRUE(mount.is_valid());
}

TEST(MountTest, BindMountEmptyPaths) {
  auto mount = Mount::BindMount("", "");
  EXPECT_TRUE(mount.is_valid());
}

// ====================
// Mount::VolumeMount Tests
// ====================

TEST(MountTest, VolumeMountBasic) {
  auto mount = Mount::VolumeMount("myvolume", "/data");
  EXPECT_TRUE(mount.is_valid());
}

TEST(MountTest, VolumeMountWithReadOnly) {
  auto mount = Mount::VolumeMount("pgdata", "/var/lib/postgresql/data").with_read_only();
  EXPECT_TRUE(mount.is_valid());
}

TEST(MountTest, VolumeMountEmptyName) {
  auto mount = Mount::VolumeMount("", "/data");
  EXPECT_TRUE(mount.is_valid());
}

// ====================
// Mount::TmpfsMount Tests
// ====================

TEST(MountTest, TmpfsMountBasic) {
  auto mount = Mount::TmpfsMount("/tmp");
  EXPECT_TRUE(mount.is_valid());
}

TEST(MountTest, TmpfsMountWithSizeBytes) {
  auto mount = Mount::TmpfsMount("/tmp").with_size_bytes(64 * 1024 * 1024);
  EXPECT_TRUE(mount.is_valid());
}

TEST(MountTest, TmpfsMountWithMode) {
  auto mount = Mount::TmpfsMount("/tmp").with_mode(0755);
  EXPECT_TRUE(mount.is_valid());
}

TEST(MountTest, TmpfsMountWithSizeAndMode) {
  auto mount = Mount::TmpfsMount("/tmp").with_size_bytes(128 * 1024 * 1024).with_mode(0700);
  EXPECT_TRUE(mount.is_valid());
}

// ====================
// Mount Fluent Chain Tests
// ====================

TEST(MountTest, FluentChainBindMountFullConfig) {
  auto mount =
      Mount::BindMount("/host/config", "/app/config").with_read_only().with_mode(0644);
  EXPECT_TRUE(mount.is_valid());
}

TEST(MountTest, FluentChainTmpfsFullConfig) {
  auto mount = Mount::TmpfsMount("/tmp/work")
                   .with_size_bytes(256 * 1024 * 1024)
                   .with_mode(0755)
                   .with_read_write();
  EXPECT_TRUE(mount.is_valid());
}

// ====================
// Mount Move Semantics Tests
// ====================

TEST(MountTest, MoveConstructor) {
  auto mount1 = Mount::BindMount("/host", "/container");
  auto mount2 = std::move(mount1);
  EXPECT_TRUE(mount2.is_valid());
}

TEST(MountTest, MoveAssignment) {
  auto mount1 = Mount::BindMount("/host1", "/container1");
  auto mount2 = Mount::BindMount("/host2", "/container2");
  mount2 = std::move(mount1);
  EXPECT_TRUE(mount2.is_valid());
}

// ====================
// Mount Edge Cases Tests
// ====================

TEST(MountTest, VeryLongPaths) {
  std::string long_path(500, 'a');
  auto mount = Mount::BindMount("/" + long_path, "/" + long_path);
  EXPECT_TRUE(mount.is_valid());
}

TEST(MountTest, PathsWithSpecialChars) {
  auto mount = Mount::BindMount("/host/my-data_123", "/container/my-data_123");
  EXPECT_TRUE(mount.is_valid());
}

TEST(MountTest, TmpfsWithZeroSize) {
  auto mount = Mount::TmpfsMount("/tmp").with_size_bytes(0);
  EXPECT_TRUE(mount.is_valid());
}

TEST(MountTest, TmpfsWithLargeSize) {
  auto mount = Mount::TmpfsMount("/tmp").with_size_bytes(4LL * 1024 * 1024 * 1024); // 4GB
  EXPECT_TRUE(mount.is_valid());
}

TEST(MountTest, ModeWithAllPermissions) {
  auto mount = Mount::TmpfsMount("/tmp").with_mode(0777);
  EXPECT_TRUE(mount.is_valid());
}

TEST(MountTest, ModeWithNoPermissions) {
  auto mount = Mount::TmpfsMount("/tmp").with_mode(0000);
  EXPECT_TRUE(mount.is_valid());
}

