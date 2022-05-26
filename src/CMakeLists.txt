
link_libraries(benchmark::benchmark)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib)


add_executable(min_max min_max.cpp)

