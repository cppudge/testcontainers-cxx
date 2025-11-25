# Developer Guide

This document is intended for developers working on the `testcontainers-cxx` library itself.

---

## Repository Layout

```
testcontainers-cxx/
├─ CMakeLists.txt
├─ .clang-format          # C++ style
├─ .editorconfig          # basic whitespace only (no eol rules)
├─ .gitattributes         # text=auto eol=lf; generated artefacts marked
├─ .gitignore
├─ conanfile.py           # Conan package recipe
├─ cmake/                 # CMake helpers
├─ cpp/
│  ├─ testcontainers/     # Public C++ headers
│  │  ├─ core/            # Core types (WaitFor, ContainerPort, Mount, etc.)
│  │  ├─ system/          # System types (Path, IpAddr, UrlHost, etc.)
│  │  └─ *.hpp/cpp        # Main API (GenericImage, Container, etc.)
│  ├─ details/            # Private implementation helpers
│  └─ utils/              # Internal utilities invisible to end users (BoxHelper, ErrorHelper, etc.)
├─ tests/
│  ├─ unit/               # Unit tests (GTest)
│  ├─ integration/        # Integration tests (require Docker)
│  └─ utils/              # Test utilities (TempFile, DockerCli)
└─ rust/
   ├─ src/
   │  ├─ lib.rs           # CXX bridge definitions
   │  └─ *.rs             # Rust wrappers around testcontainers-rs
   └─ Cargo.toml
```

---

## Building & Testing

### Build with CMake

```bash
cmake -S . -B build/native -DCMAKE_BUILD_TYPE=Release
cmake --build build/native --config Release
ctest --test-dir build/native --output-on-failure -C Release
```

### Build with Conan

```bash
# Create package with tests
conan create . --build=missing

# Skip tests during build
conan create . --build=missing -c tools.build:skip_test=True

# Build as shared library
conan create . --build=missing -o '&:shared=True'
```

### Running Tests

```bash
# Unit tests only (no Docker required)
ctest --test-dir build/native -R unit -C Release

# Integration tests (Docker required)
ctest --test-dir build/native -R integration -C Release
```

---

## Architecture

### Design Principles

* **Zero-logic bridge**: All business logic stays in `testcontainers-rs`; C++ provides only a thin wrapper
* **Type-safe ownership**: Rust objects (`rust::Box<Rs{Type}>`) managed via RAII with `std::unique_ptr` and custom deleters
* **Clean public API**: No Rust types (`rust::Box`, `rust::Vec`, etc.) exposed in public C++ headers
* **Move semantics**: Efficient ownership transfer; copy operations disabled or explicit
* **Fluent interface**: Builder pattern preserved (e.g., `.with_exposed_port(8080).with_env("KEY", "value")`)
* **Exception-based errors**: Rust `Result<T, E>` errors mapped to C++ exceptions (`testcontainers::Error`)
* **Blocking API**: All operations are synchronous; RAII ensures container cleanup

### FFI Bridge (C++ ↔ Rust)

* **Interop**: `cxx::bridge` with Rust `staticlib` (`.a`/`.lib`). No raw C ABI.
* **API shape**: 1-to-1 C++ facade for Rust internals. Blocking calls.
* **Ownership**: `Container` on C++ side holds a `std::unique_ptr<RsContainer, Deleter>`; destruction triggers Rust cleanup.
* **Errors**: Rust functions return `Result<T, String>`. By default, `cxx` maps these to `rust::Error` exceptions in C++. We wrap all non-`noexcept` Rust calls in `details::call_map_error` to convert `rust::Error` to `testcontainers::Error`.

### Type System

Complex types in this library typically have three variants:

1. **Interface type** (C++): Public API visible to end users, with interface working only with native C++ types (e.g., `GenericImage`, `Container`, `WaitFor`)
2. **Bridge type** (C++/Rust): Opaque types used in `cxx::bridge` (e.g., `RsGenericImage`, `RsContainer`, `RsWaitFor`)
3. **Rust type** (Rust): Internal Rust implementation wrapping `testcontainers-rs` types

**Naming convention**: Bridge types use `Rs` prefix (e.g., `ComplexType` → `RsComplexType`)

---

## Development Rules & Guidelines

### Core Principles

1. **Minimal logic**: All business logic resides in `testcontainers-rs`; C++ wrappers only handle FFI and RAII
2. **No Rust types in public headers**: Public C++ API must not expose `rust::Box`, `rust::Vec`, `rust::Str`, or any auto-generated `cxxbridge` types
3. **RAII everywhere**: All resources managed via constructors/destructors
4. **Explicit file listing**: All C++ source files must be explicitly listed in `CMakeLists.txt`
5. **Keep it simple**: Avoid premature optimization; prioritize maintainability and clarity

### C++ Coding Rules

#### Type Wrapping

- C++ classes wrap Rust types using `std::unique_ptr<RsType, void (*)(RsType*)>` with a custom deleter that wraps the raw pointer in `rust::Box` before calling the Rust destructor
- Forward-declare `RsType` in public headers; include `rust/cxx.h` and `rust_tc_bridge/lib.h` only in `.cpp` files
- Example:
  ```cpp
  // In .hpp (public header)
  class RsContainer; // Forward declaration
  
  class Container {
  public:
    // ...
  private:
    std::unique_ptr<RsContainer, void (*)(RsContainer *)> rimpl_;
  };
  
  // In .cpp (implementation)
  #include <rust/cxx.h>
  #include <rust_tc_bridge/lib.h>
  #include "details/BoxHelper.hpp"
  
  Container::Container(RsContainer *container) noexcept
      : rimpl_(container, [](RsContainer *c) {
          ::rs_container_destroy(details::box_from_raw(c));
        }) {}
  ```

#### Rust Function Naming Convention

Every bridge type follows a consistent naming pattern:

```rust
// Must have (RAII)
rs_complex_type_destroy(Box<RsComplexType>)

// For types with constructors
rs_complex_type_new() -> Box<RsComplexType>

// For types with Clone
rs_complex_type_clone(&RsComplexType) -> Box<RsComplexType>

// For member functions
rs_complex_type_member_function(&RsComplexType, ...) -> Result<ReturnType>
```

#### Error Handling

- All Rust functions that return `Result<T, E>` are **not** marked `noexcept` in the bridge
- All C++ functions calling such Rust functions must wrap the call in `details::call_map_error` to convert `rust::Error` to `testcontainers::Error`
- Example:
  ```cpp
  // Rust function is noexcept
  Container Container::start() {
    return Container(::rs_generic_image_start(rimpl_.get()).into_raw());
  }
  
  // Rust function returns Result<T, E>
  std::string Container::get_bridge_ip_address() {
    return details::call_map_error([this] {
      return std::string(::rs_container_get_bridge_ip_address(rimpl_.get()));
    });
  }
  ```

#### Type Mapping (C++ ↔ Rust)

| C++ Type | Rust Type | Notes |
|----------|-----------|-------|
| `std::string_view` | `&str` | Non-owning string reference |
| `std::string_view` | `String` | Owned string |
| `std::chrono::duration<uint64_t, std::nano>` | `Duration` | Use nanoseconds; convert with `Duration::from_nanos()` |
| `std::optional<T>` | `Vec<T>` (0 or 1 element) | Until `cxx` supports `Option<T>`; use `_opt` suffix in function names |
| `std::vector<T>` | `rust::Vec<T>` | For Rust objects; see helpers in `cpp/utils/VectorHelper.hpp` |
| `std::pair<T, U>` | Two separate values | `cxx` doesn't support pairs; split into two parameters |

Notes:
- `std::string_view` is used instead of `std::string` because `String` anyway needs to be copied to Rust and we have no profit from moving `std::string` into Rust.
- for `Vec<T>` we usually have to implement a helper bridge-function, like `rs_wait_for_vec_push`, because we can't hold raw `RsComplexType*` in C++ and we can't push `Box<RsComplexType>` into `Vec<RsComplexType>` in C++.

#### Additional Rules

- **Pass by value**: Use value passing instead of const references (CXX bridge limitation)
- **Null checks**: Rust cannot accept null `Box<T>` pointers; add validity checks where objects may be empty
- **Enum representation**: Rust enums with associated data are represented as C++ classes with static factory methods (see `WaitFor.hpp`, `ContainerPort.hpp`)
- **Time handling**: Always use nanoseconds for duration conversions
- **Vector of pairs**: Split into two separate vectors (CXX limitation)
- **Optional values**: Convert `std::optional<T>` to `Vec<T>` with 0-1 elements; add `_opt` suffix to function names

#### Header Include Order

1. Standard library headers (`<memory>`, `<string>`, etc.)
2. Conan dependencies (e.g., `<gtest/gtest.h>`)
3. Rust bridge headers (`<rust/cxx.h>`, `<rust_tc_bridge/lib.h>`) — **only in `.cpp` files**
4. Local project headers (`"testcontainers/..."`, `"details/..."`)

### Testing Guidelines

#### Unit Tests
- Located in `tests/unit/`
- Do **not** require Docker
- Test individual components in isolation
- Use GTest framework

#### Integration Tests
- Located in `tests/integration/`
- **Require** Docker daemon running
- Custom `main.cpp` checks Docker availability before running tests
- All custom-built images use unified naming: `testcontainers_integration_test_container:<test_name>`
- Cleanup happens after **all** tests complete in the custom `main()`

#### Test Utilities
- `TempFile`: RAII wrapper for temporary test files
- `DockerCli`: Helper for Docker CLI operations (image cleanup, availability checks)

---

## Code Style

### C++
- Follow `.clang-format` (based on LLVM style)
- Format before committing: `clang-format -i <files>`

### Rust
- Follow `rustfmt` defaults
- Format before committing: `cargo fmt`

### Comments
- All source code comments in **English only**
- Document public API with Doxygen-style comments
- Explain "why", not "what" in implementation comments

---

## TODO / Roadmap

### Short-term (v0.2)

- [ ] Format all code consistently
  - [ ] Run `clang-format` on all C++ files
  - [ ] Run `cargo fmt` on all Rust files
  - [ ] Organize includes in C++ files according to style guide

- [ ] Add missing features
  - [ ] Implement `CmdWaitFor` strategy
  - [ ] Add getters for Rust object properties where needed

### Medium-term (v0.3+)

- [ ] **LogConsumer** support
  - Requires Rust → C++ callback mechanism
  - See https://cxx.rs/ for callback patterns

- [ ] **Replace OptionHelper** with `rust::Option<T>`
  - Waiting for native `Option<T>` support in `cxx`
  - Currently using `Vec<T>` with 0-1 elements as workaround

- [ ] **C++ wrappers for testcontainers-modules**
  - Community modules: https://rust.testcontainers.org/quickstart/community_modules/
  - Redis, Postgres, MongoDB, etc.

### Long-term (v1.0+)

- [ ] **Async runner/builder**
  - Waiting for stable async support in `cxx`: https://cxx.rs/async.html
  - Non-blocking API alongside blocking RAII API

- [ ] **Static library variant**
  - Provide `.a`/`.lib` without shared library dependency
  - Investigate build configuration and symbol visibility

### Known Issues / Investigations

- [ ] **Rust rebuild with multi-config generators**
  - Rust rebuilds when switching between Debug/Release in multi-config CMake generators (MSVC, Xcode)
  - Investigate if this is expected Corrosion behavior or configuration issue
  - Low priority (doesn't affect single-config generators)

### Completed ✅

- [x] Core API implementation (GenericImage, Container, ContainerRequest, GenericBuildableImage)
- [x] Wait strategies (Log, Health, Exit, Duration)
- [x] Hide Rust types from public C++ headers
- [x] Integration tests with Docker
- [x] Unit tests for all public types
- [x] Conan package support
- [x] Cross-platform support (Windows, Linux, macOS)
- [x] Documentation

---

## Troubleshooting

### Build Issues

**Problem**: CMake can't find Rust or Corrosion  
**Solution**: Ensure Rust is in PATH; Corrosion is fetched automatically by CMake

**Problem**: Linker errors with `testcontainers-rs` symbols  
**Solution**: Ensure Rust `staticlib` is built correctly; check `target/` directory

**Problem**: Tests fail with "Docker not available"  
**Solution**: Ensure Docker daemon is running and accessible

### Development Workflow

1. Make changes to C++ or Rust code
2. Run CMake configure (if new files added)
3. Build: `cmake --build build/native --config Release`
4. Run tests: `ctest --test-dir build/native -C Release --output-on-failure`
5. Format code: `clang-format -i` (C++) and `cargo fmt` (Rust)
6. Commit changes

---

## Contributing

1. Follow the coding rules and style guidelines above
2. Add tests for new features (unit + integration)
3. Ensure all tests pass before submitting
4. Keep commits focused and write clear commit messages
5. Update documentation as needed

---

## Additional Resources

- [testcontainers-rs](https://docs.rs/testcontainers/latest/testcontainers/)
- [cxx documentation](https://cxx.rs/)
- [Corrosion](https://corrosion-rs.github.io/corrosion/)

---
