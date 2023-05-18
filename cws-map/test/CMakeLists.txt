set(PROJECT_TEST_NAME ${PROJECT_NAME}-test)


FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.13.0.zip
)
FetchContent_MakeAvailable(googletest)


file(GLOB_RECURSE TSTS CONFIGURE_DEPENDS ./*.cpp ./*.c)

add_executable(${PROJECT_TEST_NAME} ${TSTS})

target_link_libraries(${PROJECT_TEST_NAME} ${PROJECT_NAME} gtest_main)


add_test(NAME ${PROJECT_TEST_NAME} COMMAND $<TARGET_FILE:${PROJECT_TEST_NAME}> 
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
