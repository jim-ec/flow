add_executable(tests SequenceTests.cpp)
target_link_libraries(tests sequences)
enable_testing()
add_test(tests tests)
