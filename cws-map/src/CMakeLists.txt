file(GLOB_RECURSE SRCS CONFIGURE_DEPENDS
  ./*.c
  ./*.cpp
)

add_library(${PROJECT_NAME} STATIC ${SRCS})

target_include_directories(${PROJECT_NAME} PUBLIC ../include)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)

# target_compile_options(${PROJECT_NAME} PUBLIC
#   -Wall
#   $<$<CONFIG:DEBUG>:-g -O0>
#   $<$<CONFIG:SANITIZED>:-fsanitize=address,undefined,leak -g -O0>
# )
# target_link_options(${PROJECT_NAME} PUBLIC
#   -Wall
#   $<$<CONFIG:DEBUG>:-g -O0>
#   $<$<CONFIG:SANITIZED>:-fsanitize=address,undefined,leak -g -O0>
# )

install(TARGETS ${PROJECT_NAME})
