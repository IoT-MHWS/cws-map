set(PROJECT_NAME cws-map)

project(${PROJECT_NAME}
  DESCRIPTION "Coworking space map"
  LANGUAGES C CXX
)



file(GLOB_RECURSE SRCS CONFIGURE_DEPENDS
  ./*.c
  ./*.cpp
)

add_library(${PROJECT_NAME} STATIC ${SRCS})

target_include_directories(${PROJECT_NAME} PUBLIC include)
