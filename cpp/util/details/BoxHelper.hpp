#pragma once

#include <memory>

#include <rust/cxx.h>
#include <rust_tc_bridge/lib.h>

namespace testcontainers::details {

template <typename T> rust::Box<T> box_from_raw(T *raw) noexcept {
  return rust::Box<T>::from_raw(raw);
}

template <typename T>
rust::Box<T> into_box(std::unique_ptr<T, void (*)(T *)> &unique_ptr) noexcept {
  return rust::Box<T>::from_raw(unique_ptr.release());
}

inline rust::String into_string(std::string_view view) {
  return rust::String(view.data(), view.size());
}

inline rust::Str into_str(std::string_view view) {
  return rust::Str(view.data(), view.size());
}

} // namespace testcontainers::details
