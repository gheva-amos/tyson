cmake_minimum_required(VERSION 3.10)
set(PROJECT_NAME Tyson)
project(${PROJECT_NAME} CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_BUILD_TYPE Debug)

include_directories(include)

include(FetchContent)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/master.zip
    )

FetchContent_MakeAvailable(googletest)

add_subdirectory(src)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

enable_testing()
set(PROJECT_TEST_NAME test_${PROJECT_NAME})

file(GLOB_RECURSE TEST_SRC_FILES "${PROJECT_SOURCE_DIR}/tests/*.cpp")
add_executable(${PROJECT_TEST_NAME} ${TEST_SRC_FILES})
#target_include_directories(${PROJECT_TEST_NAME} PRIVATE ${boost_SOURCE_DIR}/libs/math/include)
target_compile_options(${PROJECT_TEST_NAME} PRIVATE -Wall -Wextra -pedantic -Wno-enum-compare -Wno-maybe-uninitialized -Wno-uninitialized -Wno-unused-parameter)
target_link_libraries(${PROJECT_TEST_NAME} GTest::gtest_main lexer)
include(GoogleTest)
gtest_discover_tests(${PROJECT_TEST_NAME})
