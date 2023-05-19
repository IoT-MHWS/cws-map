set(PROJECT_TEST_NAME ${PROJECT_NAME}-test)

file(GLOB_RECURSE TSTS CONFIGURE_DEPENDS ./*.cpp ./*.c)

add_executable(${PROJECT_TEST_NAME} ${TSTS})

target_link_libraries(${PROJECT_TEST_NAME} 
  GTest::gtest_main
  cws-map
  cws-proto
)

add_test(NAME ${PROJECT_TEST_NAME} 
  COMMAND $<TARGET_FILE:${PROJECT_TEST_NAME}> 
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)
