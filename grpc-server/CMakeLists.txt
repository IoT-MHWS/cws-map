set(PROJECT_NAME grpc-server)

project(${PROJECT_NAME} LANGUAGES C CXX)


if (NOT DEFINED PROTO_LOC)
  message(FATAL_ERROR "PROTO_LOC is not defined")
endif (NOT DEFINED PROTO_LOC)


if (FETCH_GRPC)
  FetchContent_Declare(
    gRPC
    GIT_REPOSITORY https://github.com/grpc/grpc
    GIT_TAG v1.54.0
  )
  FetchContent_MakeAvailable(gRPC)
endif (FETCH_GRPC)


include(cmake/custom-protobuf-generate.cmake)
include(cmake/proto.cmake)


add_executable(${PROJECT_NAME}
  ./src/main.cpp
)


target_link_libraries(${PROJECT_NAME} PRIVATE
  cws-map
  cws-proto
)
