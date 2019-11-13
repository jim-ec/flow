add_executable(tests main.cpp combinators.cpp terminators.cpp utilities.cpp mutation.cpp tools.cpp tools.h next-gen.cpp)
target_link_libraries(tests sequences)
enable_testing()
add_test(tests tests)

if (CMAKE_BUILD_TYPE MATCHES Debug AND UNIX)
    message("debug iterators")
    target_compile_definitions(tests PUBLIC _GLIBCXX_DEBUG)
endif ()

# Compile tests with C++17
set_target_properties(tests PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO)
