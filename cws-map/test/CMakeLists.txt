set(TEST_NAME cws_map_test)

find_package(GTest)

file(GLOB TSTS CONFIGURE_DEPENDS ./*.cpp ./*.c)

add_executable(${TEST_NAME} ${TSTS})

target_link_libraries(${TEST_NAME} PUBLIC cws_map GTest::gtest_main)

add_test(NAME ${TEST_NAME} 
  COMMAND $<TARGET_FILE:${TEST_NAME}> 
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)
