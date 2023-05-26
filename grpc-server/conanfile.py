import os

from conan import ConanFile
from conan.tools.cmake import CMakeToolchain
from conan.tools.cmake import cmake_layout, CMake
from conan.tools.files import copy


class cws_grpc_server(ConanFile):
    name = "cws_grpc_server"
    version = "1.0"

    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeDeps"

    _folders_rel_root = ".."
    _folders_rel_sub = "grpc-server"

    def export_sources(self):
        es_sub = os.path.join(self.export_sources_folder, self._folders_rel_sub)

        copy(self, "CMakeLists.txt", self.recipe_folder, es_sub)
        copy(self, "src/*", self.recipe_folder, es_sub)
        copy(self, "test/*", self.recipe_folder, es_sub)

    def source(self):
        pass

    def requirements(self):
        self.requires("cws_proto/1.0")
        self.requires("cws_map/1.0")
        self.requires("grpc/1.50.1")
        self.test_requires("gtest/cci.20210126")

    def layout(self):
        self.folders.root = self._folders_rel_root
        self.folders.subproject = self._folders_rel_sub
        cmake_layout(self)

    def generate(self):
        tc = CMakeToolchain(self)
        tc.generate()

    def build(self):
        # copy everything from folder with different name
        cmake = CMake(self)
        cmake.configure()
        cmake.build(["--verbose"])

    def package(self):
        cmake = CMake(self)
        cmake.install()

    def package_info(self):
        pass
