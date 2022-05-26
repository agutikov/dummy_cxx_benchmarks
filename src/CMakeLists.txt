

link_libraries(benchmark::benchmark)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib)


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

################################################################################
# Benchmarks

add_subdirectory(min_max)


################################################################################
# Tests

set(tests_src
    tests.cpp
    min_max/tests.cpp
)

add_executable(tests ${tests_src})
target_link_libraries(tests GTest::gtest GTest::gtest_main)


include(GoogleTest)

gtest_add_tests(TARGET tests)

