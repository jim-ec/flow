add_executable(tests main.cpp common.cpp common.h tests.cpp)
target_link_libraries(tests flow)
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
