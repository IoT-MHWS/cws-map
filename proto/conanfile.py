import os

from conan import ConanFile
from conan.tools.cmake import CMakeToolchain
from conan.tools.cmake import cmake_layout, CMake
from conan.tools.files import copy
from conan.tools.files.symlinks import absolute_to_relative_symlinks


class cws_proto(ConanFile):
    name = "cws_proto"
    version = "1.0"

    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeDeps"

    # hardcoded because this file will be inited from different directories
    _folders_rel_root = ".."
    _folders_rel_sub = "proto"

    def export_sources(self):
        project_root = os.path.join(self.recipe_folder, self._folders_rel_root)
        es_sub = os.path.join(self.export_sources_folder, self._folders_rel_sub)

        copy(self, "common/*", project_root, self.export_sources_folder)
        copy(self, "cmake/*", self.recipe_folder, es_sub)
        copy(self, "CMakeLists.txt", self.recipe_folder, es_sub)

    def source(self):
        pass

    def requirements(self):
        self.requires("grpc/1.50.1")

    def layout(self):
        self.folders.root = self._folders_rel_root
        self.folders.subproject = self._folders_rel_sub
        cmake_layout(self)

    def generate(self):
        tc = CMakeToolchain(self)
        tc.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()
        # cmake.build(cli_args=["--verbose"])

    def package(self):
        cmake = CMake(self)
        cmake.install()
        copy(self, pattern="*.h", 
             src=os.path.join(self.build_folder, "proto"), 
             dst=os.path.join(self.package_folder, "include"))

        absolute_to_relative_symlinks(self, self.package_folder)

    def package_info(self):
        self.cpp_info.set_property("cmake_file_name", "cws_proto")
        self.cpp_info.libs = ["cws_proto"]
        self.cpp_info.set_property("cmake_target_name", "cws_proto::cws_proto")

