find_package(Protobuf REQUIRED)
find_program(_GRPC_CPP_PLUGIN_LOCATION grpc_cpp_plugin)


set(PROTOC_OUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/cws-proto)
file(MAKE_DIRECTORY ${PROTOC_OUT_DIR})


set(PROTO_FILES
  ${PROTO_LOC}/cws-map/hello.proto
)


add_library(cws-proto ${PROTO_FILES})

target_include_directories(cws-proto PUBLIC
  ${PROTOC_OUT_DIR}
)

protobuf_generate(
  TARGET cws-proto
  LANGUAGE cpp
  IMPORT_DIRS ${Protobuf_INCLUDE_DIR}
  PROTOC_OUT_DIR ${PROTOC_OUT_DIR}
  APPEND_PATH ${PROTO_LOC}
)

protobuf_generate(
  TARGET cws-proto
  LANGUAGE grpc
  GENERATE_EXTENSIONS .grpc.pb.h .grpc.pb.cc
  PLUGIN "protoc-gen-grpc=${_GRPC_CPP_PLUGIN_LOCATION}"
  IMPORT_DIRS ${Protobuf_INCLUDE_DIR}
  PROTOC_OUT_DIR ${PROTOC_OUT_DIR}
  APPEND_PATH ${PROTO_LOC}
)
