cmake_minimum_required(VERSION 3.25)

set(PROJECT_NAME cws_map)

project(${PROJECT_NAME}
  DESCRIPTION "Coworking space map"
  LANGUAGES C CXX
)

add_subdirectory(src)

if (NOT BUILD_TESTING STREQUAL OFF)
  add_subdirectory(test)
endif()

