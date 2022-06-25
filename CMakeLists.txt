cmake_minimum_required(VERSION 3.10)

project(whirl)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)

add_library(whirl)

add_executable(whirl.cli)
set_target_properties(whirl.cli PROPERTIES OUTPUT_NAME whirl)

include(Sources.cmake)

target_link_libraries(whirl.cli whirl)
