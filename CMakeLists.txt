cmake_minimum_required(VERSION 3.15)
project(Mylib)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_BUILD_TYPE "Debug")

add_subdirectory(src)
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/./bin)
add_executable(test ./test/main.cpp)
TARGET_LINK_LIBRARIES(test mylib)

