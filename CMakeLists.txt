cmake_minimum_required(VERSION 3.15.0 FATAL_ERROR)
project(Graphics VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(PYBIND11_FINDPYTHON ON)
find_package(Python3 REQUIRED COMPONENTS Interpreter Development)
find_package(pybind11 CONFIG REQUIRED)
find_package(SDL2 REQUIRED)
# find_package(SDL2_image REQUIRED CONFIG)
find_package(OpenGL REQUIRED)
add_subdirectory(lib/glad)
add_subdirectory(lib/imgui)


pybind11_add_module(graphics src/main.cpp)

file(GLOB_RECURSE SRC_FILES "${CMAKE_SOURCE_DIR}/src/*.cpp")
add_executable(Software ${SRC_FILES})

target_include_directories(Software PRIVATE "${CMAKE_SOURCE_DIR}/include")
target_link_libraries(Software
    SDL2::SDL2
    # SDL2_image::SDL2_image
    glad
    imgui
)

