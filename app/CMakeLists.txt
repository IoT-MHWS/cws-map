set(PROJECT_NAME app)

project(${PROJECT_NAME}
  DESCRIPTION "Local app to test functionality"
  LANGUAGES C CXX
)

file(GLOB_RECURSE SRCS CONFIGURE_DEPENDS
  ./*.c
  ./*.cpp
)

add_executable(${PROJECT_NAME} ${SRCS})

target_link_libraries(${PROJECT_NAME} PRIVATE
  cws-map
  cws-proto
)

target_compile_options(${PROJECT_NAME} PUBLIC
  -Wall
  $<$<CONFIG:DEBUG>:-g -O0>
  $<$<CONFIG:RELEASE>: -O3>
  $<$<CONFIG:SANITIZED>:-fsanitize=address,undefined,leak -g>
)

target_link_options(${PROJECT_NAME} PUBLIC
  -Wall
  $<$<CONFIG:DEBUG>:-g -O0 -DNDEBUG>
  $<$<CONFIG:RELEASE>: -O3>
  $<$<CONFIG:SANITIZED>:-fsanitize=address,undefined,leak -g>
)
