

link_libraries(benchmark::benchmark)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib)


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_subdirectory(min_max)
