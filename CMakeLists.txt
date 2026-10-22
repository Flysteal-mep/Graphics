cmake_minimum_required(VERSION 3.15.0 FATAL_ERROR)
project(Graphics VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(PYBIND11_FINDPYTHON NEW)

# set(BUILD_SHARED_LIBS )
# set(GLFW_LIBRARY_TYPE )
set(GLFW_BUILD_EXAMPLES OFF)
set(GLFW_BUILD_TESTS OFF)
set(GLFW_BUILD_DOCS OFF)


add_subdirectory(libs/pybind11)
add_subdirectory(libs/glad)
add_subdirectory(libs/glfw)
# I will add glfw as a find_package(glfw3 ...)

file(GLOB_RECURSE SRC_FILES "${CMAKE_SOURCE_DIR}/src/*.cpp")
add_executable(Software ${SRC_FILES})


target_include_directories(Software PRIVATE "${CMAKE_SOURCE_DIR}/include")
target_link_libraries(Software PRIVATE
    pybind11::embed
    glfw
    glad
)
