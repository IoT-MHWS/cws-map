cmake_minimum_required(VERSION 3.25)

project(cws)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(FETCHCONTENT_QUIET OFF)

include(./cmake/dependencies.cmake)

# set(CMAKE_CXX_STANDARD 23) NOTE: not supported by lsp
set(CMAKE_CXX_FLAGS "-std=c++2b ${CMAKE_CXX_FLAGS}")
set(CMAKE_C_STANDARD 17)

enable_testing()
include(CTest)

if (NOT DEFINED PROTO_LOC)
  set(PROTO_LOC ${CMAKE_CURRENT_SOURCE_DIR}/proto)
else()
  get_filename_component(PROTO_LOC "${PROTO_LOC}" ABSOLUTE)
endif()

add_subdirectory(cws-map)

if (GRPC_SERVER)
  add_subdirectory(grpc-server)
endif (GRPC_SERVER)
