cmake_minimum_required(VERSION 3.25)

project(root)

include(./cmake/dependencies.cmake)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if (NOT ADD_CWS_MAP STREQUAL OFF)
  add_subdirectory(cws-map)
endif()

if (NOT ADD_PROTO STREQUAL OFF)
  add_subdirectory(proto)
endif()

if (NOT ADD_GRPC_SERVER STREQUAL OFF)
  add_subdirectory(grpc-server)
endif()


