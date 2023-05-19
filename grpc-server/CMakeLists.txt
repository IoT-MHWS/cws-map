set(PROJECT_NAME grpc-server)
project(${PROJECT_NAME} LANGUAGES C CXX)

if (NOT DEFINED PROTO_LOC)
  message(FATAL_ERROR "PROTO_LOC is not defined")
endif (NOT DEFINED PROTO_LOC)

include(cmake/custom-protobuf-generate.cmake)
include(cmake/proto.cmake)

file(GLOB_RECURSE SRCS CONFIGURE_DEPENDS
  ./*.c
  ./*.cpp
)

add_executable(${PROJECT_NAME} ${SRCS})

target_link_libraries(${PROJECT_NAME} PRIVATE
  cws-map
  cws-proto
)

target_include_directories(${PROJECT_NAME} PRIVATE
  ./src
)
