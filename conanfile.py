import os
from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, CMake, cmake_layout, CMakeDeps
from conan.tools.files import copy

class testcontainersRecipe(ConanFile):
    name = "testcontainers-cxx"
    version = "0.25.1-alpha.1"
    package_type = "library"

    # Optional metadata
    license = "MIT OR Apache-2.0"
    author = "Petr Slaviagin <disident47@gmail.com>"
    url = "https://github.com/cppudge/testcontainers-cxx"
    description = "Testcontainers for C++"
    topics = ("testcontainers", "testing", "containers")

    # Binary configuration
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False], "fPIC": [True, False]}
    default_options = {"shared": True, "fPIC": True}

    # Sources are located in the same place as this recipe, copy them to the recipe
    exports_sources = "CMakeLists.txt", "cmake/*", "rust/*", "!rust/target/*", "cpp/*", "tests/*", "LICENSE-MIT.txt", "LICENSE-Apache-2.0"

    def config_options(self):
        if self.settings.os == "Windows":
            self.options.rm_safe("fPIC")

    def configure(self):
        if self.options.shared:
            self.options.rm_safe("fPIC")

    def layout(self):
        cmake_layout(self)

    def build_requirements(self):
        if not self.conf.get("tools.build:skip_test", default=False):
            self.test_requires("gtest/1.14.0")
    
    def generate(self):
        deps = CMakeDeps(self)
        deps.generate()
        tc = CMakeToolchain(self)
        tc.cache_variables["SKIP_CONAN_PROVIDER_CMAKE"] = True
        tc.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        cmake = CMake(self)
        cmake.install()
        copy(self, "LICENSE*", src=self.source_folder, dst=os.path.join(self.package_folder, "licenses"))

    def package_info(self):
        self.cpp_info.set_property("cmake_target_name", "testcontainers::testcontainers")
        if self.options.shared:
            self.cpp_info.libs = ["testcontainers"]
            self.cpp_info.bindirs = ["bin"]
        else:
            self.cpp_info.libs = ["testcontainers", "rust_tc_bridge", "tc_bridge"] # on linux order matters: first the libraries with dependencies, then the libraries that provide them.
            if self.settings.os == "Windows":
                self.cpp_info.system_libs = ["ntdll"]
