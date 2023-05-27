file(GLOB_RECURSE SRCS CONFIGURE_DEPENDS
  ./*.c
  ./*.cpp
)

if (_MODULE_STANDALONE STREQUAL ON)
  find_package(cws_map REQUIRED)
  find_package(cws_proto REQUIRED)
endif()

add_executable(${PROJECT_NAME} ${SRCS})

target_link_libraries(${PROJECT_NAME} PRIVATE
  cws_map::cws_map
  cws_proto::cws_proto
)

target_include_directories(${PROJECT_NAME} PRIVATE .)
