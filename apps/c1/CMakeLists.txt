cmake_minimum_required(VERSION 2.6)

set (t0 1.3)
set (srcs_t0
      1.3.cpp)
add_executable(${t0} ${srcs_t0})
target_link_libraries(
      ${t0}
      apue
      )

