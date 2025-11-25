#pragma once

#include <functional>
#include <string>
#include <utility>

#include <rust/cxx.h>

#include "testcontainers/Error.hpp"

namespace testcontainers::details {

template <typename F, typename... Args>
decltype(auto) call_map_error(F &&f, Args &&...args) {
  try {
    return std::invoke(std::forward<F>(f), std::forward<Args>(args)...);
  } catch (const rust::Error &e) {
    throw testcontainers::Error(std::string(e.what()));
  }
}

} // namespace testcontainers::details
