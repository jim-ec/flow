add_executable(tests SequenceTests.cpp)
target_link_libraries(tests sequences)
enable_testing()
add_test(tests tests)

if(CMAKE_BUILD_TYPE MATCHES Debug AND UNIX)
    message("debug iterators")
    target_compile_definitions(tests PUBLIC _GLIBCXX_DEBUG)
endif()
