cmake_minimum_required(VERSION 3.26)

project(root)

include(./cmake/dependencies.cmake)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(cws-map)
add_subdirectory(grpc-server)
add_subdirectory(proto)
