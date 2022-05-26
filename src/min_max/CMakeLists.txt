

set(min_max_src min_max.cpp)



add_executable(min_max_0 ${min_max_src})
target_compile_options(min_max_0 PUBLIC "-O0")
target_link_options(min_max_0 PUBLIC "-O0")

add_executable(min_max_1 ${min_max_src})
target_compile_options(min_max_1 PUBLIC "-O1")
target_link_options(min_max_1 PUBLIC "-O1")

add_executable(min_max_2 ${min_max_src})
target_compile_options(min_max_2 PUBLIC "-O2")
target_link_options(min_max_2 PUBLIC "-O2")

add_executable(min_max_3 ${min_max_src})
target_compile_options(min_max_3 PUBLIC "-O3")
target_link_options(min_max_3 PUBLIC "-O3")


#TODO: add bench command target

