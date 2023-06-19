cmake_minimum_required(VERSION 3.25)

set(PROJECT_NAME cws_proto)
set(LIBRARY_NAME cws_proto)

project(${PROJECT_NAME}
  DESCRIPTION "Proto files for grpc communication"
  LANGUAGES CXX
)

find_package(Protobuf CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)

include(./cmake/custom-protobuf-generate.cmake)

get_target_property(GRPC_CPP_PLUGIN_LOCATION gRPC::grpc_cpp_plugin LOCATION)

set(PROTOC_OUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/proto)
file(MAKE_DIRECTORY ${PROTOC_OUT_DIR})

# root of protobuf project
set(CWS_PROTO_LOC ${CMAKE_CURRENT_SOURCE_DIR}/../common/proto)
if (NOT EXISTS ${CWS_PROTO_LOC})
  message(FATAL_ERROR "${CWS_PROTO_LOC} with proto files is not present")
endif()

file(GLOB_RECURSE PROTO_FILES CONFIGURE_DEPENDS
  ${CWS_PROTO_LOC}/cwspb/*.proto
)

add_library(${LIBRARY_NAME} ${PROTO_FILES})
add_library(${PROJECT_NAME}::${LIBRARY_NAME} ALIAS ${LIBRARY_NAME})

custom_protobuf_generate(
  TARGET ${LIBRARY_NAME}
  LANGUAGE cpp
  GENERATE_EXTENSIONS .pb.h .pb.cc
  IMPORT_DIRS ${CWS_PROTO_LOC} ${Protobuf_INCLUDE_DIR} 
  PROTO_ROOT ${CWS_PROTO_LOC}
  PROTOC_OUT_DIR ${PROTOC_OUT_DIR}
)

custom_protobuf_generate(
  TARGET ${LIBRARY_NAME}
  LANGUAGE grpc
  GENERATE_EXTENSIONS .grpc.pb.h .grpc.pb.cc
  PLUGIN "protoc-gen-grpc=${GRPC_CPP_PLUGIN_LOCATION}"
  IMPORT_DIRS ${CWS_PROTO_LOC} ${Protobuf_INCLUDE_DIR} 
  PROTO_ROOT ${CWS_PROTO_LOC}
  PROTOC_OUT_DIR ${PROTOC_OUT_DIR}
)

target_include_directories(${LIBRARY_NAME} PUBLIC ${PROTOC_OUT_DIR})
target_include_directories(${LIBRARY_NAME} PUBLIC ./include) # for conan

target_link_libraries(${LIBRARY_NAME} PRIVATE
  protobuf::libprotobuf
  gRPC::grpc++
  gRPC::grpc
)

install(TARGETS ${LIBRARY_NAME})
