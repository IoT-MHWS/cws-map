find_package(Protobuf REQUIRED)
find_program(_GRPC_CPP_PLUGIN_LOCATION grpc_cpp_plugin)


set(PROTOC_OUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/proto)
file(MAKE_DIRECTORY ${PROTOC_OUT_DIR})

# root of protobuf project
set(CWS_PROTO_LOC ${PROTO_LOC})

file(GLOB_RECURSE PROTO_FILES CONFIGURE_DEPENDS
  ${CWS_PROTO_LOC}/cwspb/*.proto
)

add_library(cws-proto ${PROTO_FILES})


custom_protobuf_generate(
  TARGET cws-proto
  LANGUAGE cpp
  GENERATE_EXTENSIONS .pb.h .pb.cc
  IMPORT_DIRS ${CWS_PROTO_LOC} ${Protobuf_INCLUDE_DIR} 
  PROTO_ROOT ${CWS_PROTO_LOC}
  PROTOC_OUT_DIR ${PROTOC_OUT_DIR}
)

custom_protobuf_generate(
  TARGET cws-proto
  LANGUAGE grpc
  GENERATE_EXTENSIONS .grpc.pb.h .grpc.pb.cc
  PLUGIN "protoc-gen-grpc=${_GRPC_CPP_PLUGIN_LOCATION}"
  IMPORT_DIRS ${CWS_PROTO_LOC} ${Protobuf_INCLUDE_DIR} 
  PROTO_ROOT ${CWS_PROTO_LOC}
  PROTOC_OUT_DIR ${PROTOC_OUT_DIR}
)

target_include_directories(cws-proto PUBLIC ${PROTOC_OUT_DIR})

unset(CWS_PROTO_LOC)
unset(PROTOC_OUT_DIR)
