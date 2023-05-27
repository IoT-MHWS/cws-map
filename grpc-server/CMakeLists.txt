cmake_minimum_required(VERSION 3.25)

set(PROJECT_NAME grpc_server)

project(${PROJECT_NAME}
  DESCRIPTION "Coworking grpc server"
  LANGUAGES C CXX
)


if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
  set(_MODULE_STANDALONE ON)
endif()

add_subdirectory(src)

if (NOT BUILD_TESTING STREQUAL OFF)
  add_subdirectory(test)
endif()

