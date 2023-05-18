project(cws-map
  DESCRIPTION "Coworking space map"
  LANGUAGES C CXX
  )

if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
  set(CMAKE_CONFIGURATION_TYPES Debug Release Sanitized)

  set(FETCHCONTENT_QUIET OFF)
  include(FetchContent)

  include(CTest)
  add_subdirectory(test)
endif()

add_subdirectory(src)
