cmake_minimum_required(VERSION 3.8.0 FATAL_ERROR)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
SET(CMAKE_C_COMPILER clang)
SET(CMAKE_CXX_COMPILER clang++)

project(flow)

cmake_policy(SET CMP0076 NEW)

option(FLOW_BUILD_TESTS "Build unit tests" ON)

add_subdirectory(flow)

if (${FLOW_BUILD_TESTS})
    add_subdirectory(tests)
endif ()

