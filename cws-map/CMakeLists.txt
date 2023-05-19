cmake_minimum_required(VERSION 3.25)

project(cws-map
  DESCRIPTION "Coworking space map"
  LANGUAGES C CXX
  )

add_subdirectory(src)

if (CWS_MAP_TEST)
  add_subdirectory(test)
endif()

