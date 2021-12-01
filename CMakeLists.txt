cmake_minimum_required(VERSION 3.15.0)
project(spectrogramma VERSION 0.0.1)

include(CTest)
enable_testing()

set(INC_DIR  ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(SRC_DIR  ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(TEST_DIR ${CMAKE_CURRENT_SOURCE_DIR}/test)

## stb ##
find_path(STB_INCLUDE_DIRS "stb.h")