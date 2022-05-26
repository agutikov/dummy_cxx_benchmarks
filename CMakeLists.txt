cmake_minimum_required(VERSION 3.16)

project(DummyCxxBenchmarks CXX C)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")# -Werror")

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)


add_subdirectory(googletest)
add_subdirectory(benchmark)

#TODO: find boost preprocessor

add_subdirectory(src)





