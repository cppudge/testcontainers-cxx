#pragma once

#include <rust/cxx.h>

namespace testcontainers {
namespace utils {

// ---------- conversion (T -> R) ----------
template <typename R, typename T> rust::Vec<R> vector_to_vec(const std::vector<T> &vec) {
  rust::Vec<R> rust_vec;
  rust_vec.reserve(vec.size());
  for (const auto &item : vec) {
    rust_vec.emplace_back(item);
  }
  return rust_vec;
}

template <typename R, typename T> rust::Vec<R> vector_to_vec(std::vector<T> &&vec) {
  rust::Vec<R> rust_vec;
  rust_vec.reserve(vec.size());
  for (auto &item : vec) {
    rust_vec.emplace_back(std::move(item));
  }
  return rust_vec;
}

template <typename R, typename T> std::vector<R> vec_to_vector(rust::Vec<T> &&vec) {
  return std::vector<R>(vec.begin(), vec.end());
}

// ---------- identity (T -> T) ----------
template <typename T> rust::Vec<T> vector_to_vec(const std::vector<T> &vec) {
  return vector_to_vec<T, T>(vec);
}

template <typename T> rust::Vec<T> vector_to_vec(std::vector<T> &&vec) {
  return vector_to_vec<T, T>(std::move(vec));
}

template <typename T> std::vector<T> vec_to_vector(rust::Vec<T> &&vec) {
  return vec_to_vector<T, T>(std::move(vec));
}

} // namespace utils
} // namespace testcontainers
