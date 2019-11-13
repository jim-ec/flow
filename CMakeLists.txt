cmake_minimum_required(VERSION 3.8.0 FATAL_ERROR)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
SET(CMAKE_C_COMPILER clang)
SET(CMAKE_CXX_COMPILER clang++)

project(sequences)

option(SEQUENCE_TESTING "Build unit tests" ON)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT sequences)

add_subdirectory(sequences)

if (${SEQUENCE_TESTING})
    add_subdirectory(tests)
endif ()

message(STATUS "")
