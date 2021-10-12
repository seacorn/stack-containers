cmake_minimum_required(VERSION 3.10)
project("Stack Containers" CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -pedantic")

include_directories(source)

function(target_output_directory target dir)
  set_target_properties(${target} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${dir})
endfunction()

function(add_new_test target)
  add_executable(${target} ${ARGN})
  target_output_directory(${target} tests)
  add_test(${target} tests/${target})
endfunction()

include(CTest)
enable_testing()
if(BUILD_TESTING)
  add_new_test(array-test test/ArrayTest.cpp)
endif()
