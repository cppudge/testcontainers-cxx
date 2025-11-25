#pragma once

#include <atomic>
#include <filesystem>
#include <fstream>
#include <string>

namespace testcontainers {
namespace test_utils {

/**
 * @brief Helper class to create temporary test files
 * 
 * Creates a temporary file in the system temp directory with unique name.
 * The file is automatically deleted when the object is destroyed (RAII).
 */
class TempFile {
public:
  /**
   * @brief Construct a temporary file with given content
   * @param content Content to write to the file
   */
  explicit TempFile(const std::string &content) {
    path_ = std::filesystem::temp_directory_path() /
            ("testcontainers_test_" + std::to_string(counter_++) + ".txt");
    std::ofstream ofs(path_);
    if (!ofs) {
      throw std::runtime_error("Failed to create temporary file: " + path_.string());
    }
    ofs << content;
  }

  /**
   * @brief Destructor - removes the temporary file if it exists
   */
  ~TempFile() {
    try {
      if (std::filesystem::exists(path_)) {
        std::filesystem::remove(path_);
      }
    } catch (...) {
      // Suppress exceptions in destructor
    }
  }

  /**
   * @brief Get the path to the temporary file
   * @return const reference to the filesystem path
   */
  const std::filesystem::path &path() const { return path_; }

  // Delete copy operations
  TempFile(const TempFile &) = delete;
  TempFile &operator=(const TempFile &) = delete;

  // Allow move operations
  TempFile(TempFile &&other) noexcept : path_(std::move(other.path_)) {
    other.path_.clear();
  }

  TempFile &operator=(TempFile &&other) noexcept {
    if (this != &other) {
      // Clean up current file
      if (std::filesystem::exists(path_)) {
        std::filesystem::remove(path_);
      }
      path_ = std::move(other.path_);
      other.path_.clear();
    }
    return *this;
  }

private:
  std::filesystem::path path_;
  static std::atomic<int> counter_;
};

} // namespace test_utils
} // namespace testcontainers

