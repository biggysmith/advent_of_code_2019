cmake_minimum_required(VERSION 3.5.1)

project(advent_of_code_2019)

set(CMAKE_CXX_STANDARD 17)

add_executable(day01 src/day01/day1.cpp)
add_executable(day02 src/day02/day2.cpp)
add_executable(day03 src/day03/day3.cpp)
add_executable(day04 src/day04/day4.cpp)
add_executable(day05 src/day05/day5.cpp)
add_executable(day06 src/day06/day6.cpp)

#if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC" AND CMAKE_BUILD_TYPE MATCHES "Release")
#   target_compile_options(day06 PRIVATE /Zi)
#
#   # Tell linker to include symbol data
#    set_target_properties(day06 PROPERTIES 
#        LINK_FLAGS "/INCREMENTAL:NO /DEBUG /OPT:REF /OPT:ICF"
#    )
#
#    # Set file name & location
#    set_target_properties(day06 PROPERTIES 
#        COMPILE_PDB_NAME day06
#        COMPILE_PDB_OUTPUT_DIR ${CMAKE_BINARY_DIR}
#    )
#endif()