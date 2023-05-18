project(app-test
  DESCRIPTION "Small examples for testing"
  LANGUAGES C CXX
)

file(GLOB_RECURSE SRCS CONFIGURE_DEPENDS
  ./main.cpp
  # ./*.c
  # ./*.cpp
)

add_executable(${PROJECT_NAME} ${SRCS})

target_link_libraries(${PROJECT_NAME} PRIVATE
  cws-map
  # cws-proto
)

include(./cmake/illumination.cmake)
