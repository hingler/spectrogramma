

cmake_minimum_required(VERSION 3.17.0)
project(spectrogramma VERSION 0.0.1)

include(CTest)
enable_testing()

set(INC_DIR  ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(SRC_DIR  ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(TEST_DIR ${CMAKE_CURRENT_SOURCE_DIR}/test)
set(LIB_DIR  ${CMAKE_CURRENT_SOURCE_DIR}/lib)

if(MSVC)
  add_compile_options(/W3)
else()
  add_compile_options(-Wall)
endif()

## stb ##
find_path(STB_INCLUDE_DIRS "stb/stb.h")
find_package(GTest CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

add_library(fftlib ${SRC_DIR}/FFT.cpp ${SRC_DIR}/ImageData.cpp ${SRC_DIR}/STBImage.cpp ${SRC_DIR}/ImagePtr.cpp ${SRC_DIR}/fft_convert.cpp ${SRC_DIR}/audio_write.cpp)

target_link_libraries(fftlib PUBLIC glm::glm)
target_include_directories(fftlib PUBLIC ${INC_DIR} ${LIB_DIR})
target_include_directories(fftlib PUBLIC ${STB_INCLUDE_DIRS})

add_custom_command(TARGET fftlib POST_BUILD
COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/res $<TARGET_FILE_DIR:fftlib>/res)

add_executable(spectrogramma ${SRC_DIR}/main.cpp)
target_link_libraries(spectrogramma PRIVATE fftlib)

set(TEST_PATHS ${TEST_DIR}/BitReverseTest.cpp
               ${TEST_DIR}/FFTTest.cpp
               ${TEST_DIR}/ImageDataTest.cpp)

set(TEST_NAMES BitReverseTest
               FFTTest
               ImageDataTest)

foreach(test_path test_name IN ZIP_LISTS TEST_PATHS TEST_NAMES)
  add_executable(${test_name} ${test_path})
  target_link_libraries(${test_name} PRIVATE GTest::gtest_main fftlib)
  target_include_directories(${test_name} PUBLIC ${INC_DIR} ${STB_INCLUDE_DIRS})
  add_test(NAME ${test_name} COMMAND ${test_name} WORKING_DIRECTORY $<TARGET_FILE_DIR:fftlib>)
endforeach()

include(CPack)
