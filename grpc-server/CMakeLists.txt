project(grpc-server
  DESCRIPTION "Coworking grpc server"
  LANGUAGES C CXX
)

include(./cmake/custom-protobuf-generate.cmake)
include(./cmake/proto.cmake)

add_subdirectory(src)

if (GRPC_SERVER_TEST)
  add_subdirectory(test)
endif()

