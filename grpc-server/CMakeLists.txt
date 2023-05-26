cmake_minimum_required(VERSION 3.25)

set(PROJECT_NAME grpc-server)

project(${PROJECT_NAME}
  DESCRIPTION "Coworking grpc server"
  LANGUAGES C CXX
)

find_package(cws_map CONFIG REQUIRED)
find_package(cws_proto CONFIG REQUIRED)

add_subdirectory(src)

if (NOT BUILD_TESTING STREQUAL OFF)
  add_subdirectory(test)
endif()

