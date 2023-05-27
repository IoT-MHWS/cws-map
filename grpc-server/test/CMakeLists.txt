set(PROJECT_TEST_NAME ${PROJECT_NAME}_test)

find_package(GTest CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)

if (_MODULE_STANDALONE STREQUAL ON)
  find_package(cws_map REQUIRED)
  find_package(cws_proto REQUIRED)
endif()

file(GLOB_RECURSE TSTS CONFIGURE_DEPENDS ./*.cpp ./*.c)

add_executable(${PROJECT_TEST_NAME} ${TSTS})

target_link_libraries(${PROJECT_TEST_NAME} PUBLIC
  GTest::gtest_main
  cws_map::cws_map
  cws_proto::cws_proto
  gRPC::grpc++
  gRPC::grpc
)

add_test(NAME ${PROJECT_TEST_NAME} 
  COMMAND $<TARGET_FILE:${PROJECT_TEST_NAME}> 
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)
