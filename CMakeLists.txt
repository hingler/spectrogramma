

cmake_minimum_required(VERSION 3.17.0)
project(spectrogramma VERSION 0.0.1)

include(CTest)
enable_testing()

set(INC_DIR  ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(SRC_DIR  ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(TEST_DIR ${CMAKE_CURRENT_SOURCE_DIR}/test)

## stb ##
find_path(STB_INCLUDE_DIRS "stb.h")
find_package(GTest CONFIG REQUIRED)

add_library(fftlib ${SRC_DIR}/FFT.cpp)
target_include_directories(fftlib PUBLIC ${INC_DIR})

set(TEST_PATHS ${TEST_DIR}/BitReverseTest.cpp
               ${TEST_DIR}/FFTTest.cpp)

set(TEST_NAMES BitReverseTest
               FFTTest)

foreach(test_path test_name IN ZIP_LISTS TEST_PATHS TEST_NAMES)
  add_executable(${test_name} ${test_path})
  target_link_libraries(${test_name} PRIVATE GTest::gtest_main fftlib)
  target_include_directories(${test_name} PUBLIC ${INC_DIR})
  add_test(NAME ${test_name} COMMAND ${test_name})
endforeach()

include(CPack)
