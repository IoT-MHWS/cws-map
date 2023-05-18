cmake_minimum_required(VERSION 3.25)

project(cws-map
  DESCRIPTION "Coworking space map"
  LANGUAGES C CXX
  )

if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

  include(CTest)
  add_subdirectory(test)
endif()

add_subdirectory(src)
