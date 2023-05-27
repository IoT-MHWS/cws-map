file(GLOB_RECURSE SRCS CONFIGURE_DEPENDS
  ./*.c
  ./*.cpp
)

find_package(gRPC CONFIG REQUIRED)

if (_MODULE_STANDALONE STREQUAL ON)
  find_package(cws_map REQUIRED)
  find_package(cws_proto REQUIRED)
endif()

add_executable(${PROJECT_NAME} ${SRCS})

target_link_libraries(${PROJECT_NAME} PRIVATE
  cws_map::cws_map
  cws_proto::cws_proto
  gRPC::grpc++
  gRPC::grpc
)

target_include_directories(${PROJECT_NAME} PRIVATE .)
