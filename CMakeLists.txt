cmake_minimum_required(VERSION 3.25)


project(cws)


set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


if (NOT DEFINED PROTO_LOC)
  set(PROTO_LOC ${CMAKE_CURRENT_SOURCE_DIR}/proto)
endif (NOT DEFINED PROTO_LOC)


add_subdirectory(cws)

if (GRPC)
  add_subdirectory(grpc-server)
endif (GRPC)
