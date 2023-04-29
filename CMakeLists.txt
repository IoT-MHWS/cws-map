cmake_minimum_required(VERSION 3.25)


project(cws)


set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(FETCHCONTENT_QUIET OFF)
include(FetchContent)


if (NOT DEFINED PROTO_LOC)
  set(PROTO_LOC ${CMAKE_CURRENT_SOURCE_DIR}/proto)
else()
  get_filename_component(PROTO_LOC "${CMAKE_CURRENT_SOURCE_DIR}/${PROTO_LOC}" ABSOLUTE)
endif()


if (APP)
  add_subdirectory(app)
endif (APP)

add_subdirectory(cws-map)

if (GRPC_SERVER)
  add_subdirectory(grpc-server)
endif (GRPC_SERVER)
