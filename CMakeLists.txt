cmake_minimum_required(VERSION 3.28 FATAL_ERROR)
project(Graphics VERSION 0.0.0 LANGUAGES C)

set(CMAKE_C_STANDARD 23)


add_subdirectory(libs)


file(GLOB_RECURSE SRC_FILES "${CMAKE_SOURCE_DIR}/src/*.c")
add_executable(Software "${SRC_FILES}")

target_include_directories(Software PUBLIC "${CMAKE_SOURCE_DIR}/include")
target_link_libraries(Software PRIVATE projectLibs)

if(DEFINED OUTPUT_NAME)
    set_target_properties(Software PROPERTIES OUTPUT_NAME ${OUTPUT_NAME})
endif()
