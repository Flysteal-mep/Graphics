cmake_minimum_required(VERSION 3.15.0 FATAL_ERROR)
project(Graphics VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


add_subdirectory(libs)


file(GLOB_RECURSE SRC_FILES "${CMAKE_SOURCE_DIR}/src/*.cpp")
add_executable(Software ${SRC_FILES})


target_include_directories(Software PRIVATE "${CMAKE_SOURCE_DIR}/include")
target_link_libraries(Software PRIVATE projectLibs)

