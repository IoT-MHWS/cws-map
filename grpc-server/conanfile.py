import os

from conan import ConanFile
from conan.tools.cmake import CMakeToolchain
from conan.tools.cmake import cmake_layout, CMake
from conan.tools.files import copy


class hello(ConanFile):
    name = "cws_grpc_server"
    version = "1.0"

    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeDeps"

    def export_sources(self):
        es_proto_cpp_folder = os.path.join(self.export_sources_folder, self.name)
        copy(self, "conanfile.py", self.recipe_folder, es_proto_cpp_folder)
        copy(self, "CMakeLists.txt", self.recipe_folder, es_proto_cpp_folder)
        # sources
        copy(self, "src/*", self.recipe_folder, es_proto_cpp_folder)
        copy(self, "test/*", self.recipe_folder, es_proto_cpp_folder)

    def source(self):
        pass

    def requirements(self):
        self.requires("cws_proto/1.0")
        self.requires("cws_map/1.0")
        self.requires("grpc/1.50.1")
        self.test_requires("gtest/cci.20210126")

    def layout(self):
        self.folders.root = ".."
        self.folders.subproject = self.name
        cmake_layout(self)

    def generate(self):
        tc = CMakeToolchain(self)
        tc.generate()

    def build(self):
        # copy everything from folder with different name
        cmake = CMake(self)
        while(1):
            pass
        cmake.configure()
        cmake.build(["--verbose"])

    def package(self):
        cmake = CMake(self)
        cmake.install()

    def package_info(self):
        pass
