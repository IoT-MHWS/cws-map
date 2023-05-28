set(TEST_NAME ${PROJECT_NAME}_test)

find_package(GTest)

file(GLOB TSTS CONFIGURE_DEPENDS ./*.cpp ./*.c)

add_executable(${TEST_NAME} ${TSTS})

target_link_libraries(${TEST_NAME} PUBLIC cws_map GTest::gtest_main)

# Other executables
include(./algo/illumination.cmake)

add_test(NAME ${TEST_NAME} 
  COMMAND $<TARGET_FILE:${TEST_NAME}> 
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)
