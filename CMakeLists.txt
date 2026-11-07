cmake_minimum_required(VERSION 3.28 FATAL_ERROR)
project(Graphics VERSION 0.0.0 LANGUAGES C)

set(CMAKE_C_STANDARD 11)

# hard coded for now
# option(ENABLE_MSAN "Enable MemorySanitizer" ON)
# if(ENABLE_MSAN AND CMAKE_BUILD_TYPE STREQUAL "Debug")
#     if(NOT CMAKE_C_COMPILER_ID MATCHES "Clang")
#         message(FATAL_ERROR "MemorySanitizer requires Clang")
#     endif()

#     add_compile_options(
#         -O1
#         -g
#         -fsanitize=leak
#         # -fsanitize=memory
#         # -fno-omit-frame-pointer
#         # -fsanitize-memory-track-origins=2
#     )
#     add_link_options(-fsanitize=leak)
# endif()


add_subdirectory(libs)


file(GLOB_RECURSE SRC_FILES "${CMAKE_SOURCE_DIR}/src/*.c")
add_executable(Software "${SRC_FILES}")

target_include_directories(Software PUBLIC "${CMAKE_SOURCE_DIR}/include")
target_link_libraries(Software PRIVATE projectLibs)

if(DEFINED OUTPUT_NAME)
    set_target_properties(Software PROPERTIES OUTPUT_NAME ${OUTPUT_NAME})
endif()
