import os

from conan import ConanFile
from conan.tools.cmake import CMakeToolchain
from conan.tools.cmake import cmake_layout, CMake
from conan.tools.files import copy


class cws_proto(ConanFile):
    name = "cws_proto"
    version = "1.0"

    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeDeps"

    def export_sources(self):
        source_folder = os.path.join(self.recipe_folder, "..")
        copy(self, "common/*", source_folder, self.export_sources_folder)
        # copy to es dir
        es_proto_cpp_folder = os.path.join(self.export_sources_folder, self.name)
        copy(self, "conanfile.py", self.recipe_folder, es_proto_cpp_folder)
        copy(self, "cmake/*", self.recipe_folder, es_proto_cpp_folder)
        copy(self, "CMakeLists.txt", self.recipe_folder, es_proto_cpp_folder)

    def source(self):
        pass

    def requirements(self):
        self.requires("grpc/1.50.1")

    def layout(self):
        self.folders.root = ".."
        self.folders.subproject = self.name
        cmake_layout(self)

    def generate(self):
        tc = CMakeToolchain(self)
        tc.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build(cli_args=["--verbose"])

    def package(self):
        cmake = CMake(self)
        cmake.install()
        copy(self, pattern="*.h", 
             src=os.path.join(self.build_folder, "proto"), 
             dst=os.path.join(self.package_folder, "include"))

    def package_info(self):
        self.cpp_info.set_property("cmake_file_name", "cws_proto")
        self.cpp_info.libs = ["cws_proto"]
        self.cpp_info.set_property("cmake_target_name", "cws_proto::cws_proto")

