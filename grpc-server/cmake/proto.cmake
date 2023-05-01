find_package(Protobuf REQUIRED)
find_program(_GRPC_CPP_PLUGIN_LOCATION grpc_cpp_plugin)


set(PROTOC_OUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/cws-proto)
file(MAKE_DIRECTORY ${PROTOC_OUT_DIR})


# set(PROTO_FILES 
#   ${PROTO_LOC}/cws-map/hello.proto
# )

set(CWS_PROTO_LOC ${PROTO_LOC}/cws-map)

file(GLOB_RECURSE PROTO_FILES CONFIGURE_DEPENDS
  ${CWS_PROTO_LOC}/*.proto
)


set(GENERATED_SRCS)

# properly manage output dirs
foreach(PROTO_FILE ${PROTO_FILES})
  get_filename_component(FILE_WE ${PROTO_FILE} NAME_WE)
  get_filename_component(FILE_DIR ${PROTO_FILE} DIRECTORY)

  file(RELATIVE_PATH FILE_RELPATH ${CWS_PROTO_LOC} ${PROTO_FILE})
  get_filename_component(DIR_RELPATH ${FILE_RELPATH} DIRECTORY)

  set(PROTOC_OUT_REAL_DIR ${PROTOC_OUT_DIR}/${DIR_RELPATH})

  set(GENERATED_SRC)

  protobuf_generate(
    PROTOS ${PROTO_FILE}
    OUT_VAR GENERATED_SRC
    LANGUAGE cpp
    IMPORT_DIRS ${Protobuf_INCLUDE_DIR}
    PROTOC_OUT_DIR ${PROTOC_OUT_REAL_DIR}
    APPEND_PATH ${PROTO_LOC}

  )
  list(APPEND GENERATED_SRCS ${GENERATED_SRC})

  protobuf_generate(
    PROTOS ${PROTO_FILE}
    OUT_VAR GENERATED_SRC
    LANGUAGE grpc
    GENERATE_EXTENSIONS .grpc.pb.h .grpc.pb.cc
    PLUGIN "protoc-gen-grpc=${_GRPC_CPP_PLUGIN_LOCATION}"
    IMPORT_DIRS ${Protobuf_INCLUDE_DIR}
    PROTOC_OUT_DIR ${PROTOC_OUT_REAL_DIR}
    APPEND_PATH ${PROTO_LOC}
  )

  list(APPEND GENERATED_SRCS ${GENERATED_SRC})
endforeach()

add_library(cws-proto ${GENERATED_SRCS})

target_include_directories(cws-proto PUBLIC
  ${PROTOC_OUT_DIR}
)
