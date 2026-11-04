cmake_minimum_required(VERSION 3.28.0 FATAL_ERROR)


add_subdirectory(glad)

set(BUILD_SHARED_LIBS OFF)
set(GLFW_BUILD_EXAMPLES OFF)
set(GLFW_BUILD_TESTS OFF)
set(GLFW_BUILD_DOCS OFF)
set(GLFW_INSTALL OFF)
add_subdirectory(glfw)


add_library(projectLibs INTERFACE)

target_link_libraries(projectLibs INTERFACE
    glad
    glfw
)
