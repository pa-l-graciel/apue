cmake_minimum_required(VERSION 2.6)

include_directories(${CMAKE_SOURCE_DIR}/../../include)
link_directories(${CMAKE_SOURCE_DIR}/../../lib)

set (t0 1.3_simple_ls)
set (srcs_t0 1.3_simple_ls.cpp)
add_executable(${t0} ${srcs_t0})
target_link_libraries(${t0} apue)
target_compile_options( ${t0} PUBLIC -std=c++11)
