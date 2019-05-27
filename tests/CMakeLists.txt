add_executable(tests main.cpp combinators.cpp terminators.cpp containers.cpp utilities.cpp mutation.cpp tools.cpp tools.h)
target_link_libraries(tests sequences)
enable_testing()
add_test(tests tests)

if(CMAKE_BUILD_TYPE MATCHES Debug AND UNIX)
    message("debug iterators")
    target_compile_definitions(tests PUBLIC _GLIBCXX_DEBUG)
endif()
