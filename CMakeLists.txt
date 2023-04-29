cmake_minimum_required(VERSION 3.25)

set(PROJECT_NAME cws-map)

project(${PROJECT_NAME} LANGUAGES C CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_FLAGS "-std=c++2b ${CMAKE_CXX_FLAGS}")
set(CMAKE_C_FLAGS "-std=c2x ${CMAKE_C_FLAGS}")

add_subdirectory(cws)
add_subdirectory(grpc-server)
