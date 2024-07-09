cmake_minimum_required(VERSION 3.9 FATAL_ERROR)
project(jpower)

set(CMAKE_C_FLAGS "-pthread -std=gnu99")

include_directories(${PROJECT_SOURCE_DIR}/src/include)
file(GLOB SOURCES ${PROJECT_SOURCE_DIR}/src/*.c)

add_library(jpower ${SOURCES})

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

add_executable(test test.c)
target_link_libraries(test jpower)