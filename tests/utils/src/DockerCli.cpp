#include "testutils/DockerCli.hpp"

#include <cstdlib>
#include <array>
#include <memory>
#include <sstream>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

namespace testcontainers {
namespace test_utils {

namespace {

// Execute command and capture output
std::string exec_command(const char* cmd) {
  std::array<char, 128> buffer;
  std::string result;
  
#ifdef _WIN32
  std::unique_ptr<FILE, decltype(&_pclose)> pipe(_popen(cmd, "r"), _pclose);
#else
  std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
#endif

  if (!pipe) {
    return "";
  }

  while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
    result += buffer.data();
  }

  return result;
}

// Execute command silently (discard output)
int exec_command_silent(const std::string& cmd) {
#ifdef _WIN32
  std::string silent_cmd = cmd + " >NUL 2>&1";
#else
  std::string silent_cmd = cmd + " >/dev/null 2>&1";
#endif
  return std::system(silent_cmd.c_str());
}

} // anonymous namespace

bool DockerCli::remove_images_by_name(const std::string &image_name) {
  if (!is_docker_available()) {
    return false;
  }

  // Get list of image IDs matching the name
  std::string list_cmd = "docker images \"" + image_name + "\" -q";
  std::string image_ids = exec_command(list_cmd.c_str());

  if (image_ids.empty()) {
    // No images found, consider it success
    return true;
  }

  // Build command to remove all images
  std::stringstream remove_cmd;
  remove_cmd << "docker image rm -f";
  
  std::istringstream iss(image_ids);
  std::string image_id;
  while (std::getline(iss, image_id)) {
    if (!image_id.empty()) {
      remove_cmd << " " << image_id;
    }
  }

  int result = exec_command_silent(remove_cmd.str());
  return result == 0;
}

bool DockerCli::remove_image(const std::string &name, const std::string &tag) {
  if (!is_docker_available()) {
    return false;
  }

  std::string full_name = name + ":" + tag;
  std::string cmd = "docker image rm -f " + full_name;
  int result = exec_command_silent(cmd);
  return result == 0;
}

bool DockerCli::is_docker_available() {
  int result = exec_command_silent("docker version");
  return result == 0;
}

} // namespace test_utils
} // namespace testcontainers

