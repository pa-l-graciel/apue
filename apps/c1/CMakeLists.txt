cmake_minimum_required(VERSION 2.6)

include(./../../common.cmake)
include_directories(${CMAKE_SOURCE_DIR}/../../include)
link_directories(${CMAKE_SOURCE_DIR}/../../lib)

if (FALSE)
set (t0 1.3_simple_ls)
set (srcs_t0 1.3_simple_ls.cpp)
add_executable(${t0} ${srcs_t0})
target_link_libraries(${t0} apue)
target_compile_options( ${t0} PUBLIC -std=c++11)
endif()


# .cpp인 소스코드들을 자동으로 개별 빌드
SRCLIST(cpp_list ${CMAKE_SOURCE_DIR})
foreach(file_i ${cpp_list})
	string(REPLACE ".cpp" "" target_name ${file_i})
  set (target ${target_name})
  set (srcs_target ${file_i})
  add_executable(${target} ${srcs_target})
  target_link_libraries(${target} apue)
  target_compile_options( ${target} PUBLIC -std=c++11)
endforeach(file_i)

