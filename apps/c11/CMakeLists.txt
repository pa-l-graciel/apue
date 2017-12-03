cmake_minimum_required(VERSION 2.6)

include(./../../common.cmake)
include_directories(${CMAKE_SOURCE_DIR}/../../include)
link_directories(${CMAKE_SOURCE_DIR}/../../lib)
set(CMAKE_BUILD_TYPE Debug)

# .cpp인 소스코드들을 자동으로 개별 빌드
SRCLIST(cpp_list ${CMAKE_SOURCE_DIR})
foreach(file_i ${cpp_list})
	string(REPLACE ".cpp" "" target_name ${file_i})
  set (target ${target_name})
  set (srcs_target ${file_i})
  add_executable(${target} ${srcs_target})
  target_link_libraries(${target} apue pthread)
  target_compile_options( ${target} PUBLIC -std=c++11 -g)
endforeach(file_i)

