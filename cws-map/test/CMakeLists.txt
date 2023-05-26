set(PROJECT_TEST_NAME ${PROJECT_NAME}-test)

find_package(GTest)

file(GLOB TSTS CONFIGURE_DEPENDS ./*.cpp ./*.c)

add_executable(${PROJECT_TEST_NAME} ${TSTS})

target_link_libraries(${PROJECT_TEST_NAME} PUBLIC ${PROJECT_NAME} GTest::gtest_main)

add_test(NAME ${PROJECT_TEST_NAME} 
  COMMAND $<TARGET_FILE:${PROJECT_TEST_NAME}> 
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)
