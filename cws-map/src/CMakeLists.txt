set(PROJECT_LIB_NAME ${PROJECT_NAME})

file(GLOB_RECURSE SRCS CONFIGURE_DEPENDS
  ./*.c
  ./*.cpp
)

add_library(${PROJECT_LIB_NAME} STATIC ${SRCS})

target_include_directories(${PROJECT_LIB_NAME} PUBLIC ../include)

target_compile_features(${PROJECT_LIB_NAME} PUBLIC cxx_std_20)

# target_compile_options(${PROJECT_LIB_NAME} PUBLIC
#   -Wall
#   $<$<CONFIG:DEBUG>:-g -O0>
#   $<$<CONFIG:SANITIZED>:-fsanitize=address,undefined,leak -g -O0>
# )
# target_link_options(${PROJECT_LIB_NAME} PUBLIC
#   -Wall
#   $<$<CONFIG:DEBUG>:-g -O0>
#   $<$<CONFIG:SANITIZED>:-fsanitize=address,undefined,leak -g -O0>
# )
