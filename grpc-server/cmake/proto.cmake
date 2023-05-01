find_package(Protobuf REQUIRED)
find_program(_GRPC_CPP_PLUGIN_LOCATION grpc_cpp_plugin)


set(PROTOC_OUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/cws-proto/cws-map)
file(MAKE_DIRECTORY ${PROTOC_OUT_DIR})


set(CWS_PROTO_LOC ${PROTO_LOC}/cws-map)

file(GLOB PROTO_FILES CONFIGURE_DEPENDS
  ${CWS_PROTO_LOC}/*.proto
)

add_library(cws-proto ${PROTO_FILES})

# only works without nesting
protobuf_generate(
  TARGET cws-proto
  LANGUAGE cpp
  IMPORT_DIRS ${Protobuf_INCLUDE_DIR} ${CWS_PROTO_LOC}
  PROTOC_OUT_DIR ${PROTOC_OUT_DIR}
)

protobuf_generate(
  TARGET cws-proto
  LANGUAGE grpc
  GENERATE_EXTENSIONS .grpc.pb.h .grpc.pb.cc
  PLUGIN "protoc-gen-grpc=${_GRPC_CPP_PLUGIN_LOCATION}"
  IMPORT_DIRS ${Protobuf_INCLUDE_DIR} ${CWS_PROTO_LOC}
  PROTOC_OUT_DIR ${PROTOC_OUT_DIR}
)

target_include_directories(cws-proto PUBLIC
  ${PROTOC_OUT_DIR}
)
