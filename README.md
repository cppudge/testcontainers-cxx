# testcontainers-cxx

C++ bridge to [testcontainers-rs](https://github.com/testcontainers/testcontainers-rs).
Provides a **blocking**, RAII-friendly C++ API that mirrors the Testcontainers model (`BuildableGenericImage`, `GenericImage`, `ContainerRequest`, `Container`) while delegating all implementation to Rust.

Native support for Windows, Linux, macOS and cross-compilation to other architectures.

Major.Minor version of this library = Major.Minor version of testcontainers-rs, Patch version of this library used for internal changes and bug fixes.

---

## Usage

You can find the example in the [test_package](./test_package) directory.

Typical usage in C++ looks like this:

```python
# conanfile.py
def requirements(self):
    self.requires("testcontainers-cxx/0.25.1")
```

```cmake
# CMakeLists.txt
find_package(testcontainers-cxx CONFIG REQUIRED)
target_link_libraries(my_target PRIVATE testcontainers::testcontainers)
```

```cpp
// example.cpp
#include <testcontainers/testcontainers.hpp>

using namespace testcontainers;

auto container = GenericImage("redis", "7.2.4")
  .with_exposed_port(6379)
  .with_wait_for(WaitFor::message_on_stdout("Ready to accept connections"))
  .start();

auto host_port = container.get_host_port_ipv4(ContainerPort::Tcp(6379));

// container will be automatically stopped when it goes out of scope
```

## Building

You can build this library yourself from conan:

```bash
conan create . --build=* -o '&:shared=True' -c tools.build:skip_test=True
```

or from cmake:

```bash
cmake -S . -B build/native -DCMAKE_BUILD_TYPE=Release
cmake --build build/native --config Release
ctest --test-dir build/native --output-on-failure -C Release
```

### Requirements
* Rust
* CMake
* Conan 2.x
* C++ compiler (MSVC on Windows, Clang/GCC on Linux/macOS)
* Optional: GTest for unit and integration tests
* Optional: Docker for integration tests

Optional dependencies can be disabled by setting `-c tools.build:skip_test=True`.

---

## Development

see [README_DEV.md](./README_DEV.md)

---

# Thanks to

* [Cursor](https://github.com/cursor/cursor) and [VSCode](https://github.com/microsoft/vscode)
* [CMake](https://github.com/Kitware/CMake)
* [Conan](https://github.com/conan-io/conan)
* [CMake Conan](https://github.com/conan-io/cmake-conan)
* [Corrosion](https://github.com/corrosion-rs/corrosion)
* [cxx](https://github.com/dtolnay/cxx)
* [Rust](https://github.com/rust-lang/rust)
* [Testcontainers](https://github.com/testcontainers/testcontainers-rs)
* [GTest](https://github.com/google/googletest)
* and all the other contributors to the projects that made this possible

## License

Licensed under either of

- Apache License, Version 2.0
  ([LICENSE-APACHE](LICENSE-Apache-2.0) or http://www.apache.org/licenses/LICENSE-2.0)
- MIT license
  ([LICENSE-MIT](LICENSE-MIT) or http://opensource.org/licenses/MIT)

at your option.
