#pragma once

#include <optional>
#include <rust/cxx.h>

namespace testcontainers {
namespace utils {

// ---------- conversion (T -> R) ----------
template <typename R, typename T> rust::Vec<R> optional_to_vec(std::optional<T> &&opt) {
  return opt.has_value() ? rust::Vec<R>({R(std::move(opt.value()))}) : rust::Vec<R>();
}

template <typename R, typename T, typename F>
rust::Vec<R> optional_to_vec(std::optional<T> &&opt, F &&transform) {
  if (opt.has_value())
    return rust::Vec<R>({std::forward<F>(transform)(std::move(opt.value()))});
  return rust::Vec<R>();
}

template <typename R, typename T> std::optional<R> vec_to_optional(rust::Vec<T> &&vec) {
  if (vec.empty()) {
    return std::nullopt;
  }
  return R(std::move(vec[0]));
}

// ---------- identity (T -> T) ----------
template <typename T> rust::Vec<T> optional_to_vec(std::optional<T> &&opt) {
  return optional_to_vec<T, T>(std::move(opt));
}

template <typename T> std::optional<T> vec_to_optional(rust::Vec<T> &&vec) {
  return vec_to_optional<T, T>(std::move(vec));
}

} // namespace utils
} // namespace testcontainers
