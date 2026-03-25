cmake_minimum_required(VERSION 3.16)
set (CMAKE_CXX_STANDARD 17)
project(LSM6DSOX_test LANGUAGES CXX)

find_package(GTest CONFIG REQUIRED)

include(CTest)
include(FetchContent)

include_directories(${GTEST_INCLUDE_DIRS})  # Var managed by CMake
include(GoogleTest)
message(STATUS "GTest include dirs: ${GTEST_INCLUDE_DIRS}")


add_executable(unit_test unit_test.cpp)
target_link_libraries(unit_test LSM6DSOX_Rpi)
target_link_libraries(unit_test gtest gtest_main)

add_executable(main main.cpp)
target_link_libraries(main LSM6DSOX_Rpi)

gtest_discover_tests(unit_test)

add_compile_options(-Wall -pedantic)

set(CMAKE_BUILD_TYPE Debug)