set(LIBRARY_NAME cws_map)

file(GLOB_RECURSE SRCS CONFIGURE_DEPENDS
  ./*.c
  ./*.cpp
)

add_library(${LIBRARY_NAME} STATIC ${SRCS})
add_library(${PROJECT_NAME}::${LIBRARY_NAME} ALIAS ${LIBRARY_NAME})

target_include_directories(${LIBRARY_NAME} PUBLIC ../include)

target_compile_features(${LIBRARY_NAME} PUBLIC cxx_std_20)

install(TARGETS ${LIBRARY_NAME})
