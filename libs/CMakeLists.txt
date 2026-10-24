set(PYBIND11_FINDPYTHON NEW)

# set(BUILD_SHARED_LIBS )
# set(GLFW_LIBRARY_TYPE )
set(GLFW_BUILD_EXAMPLES OFF)
set(GLFW_BUILD_TESTS OFF)
set(GLFW_BUILD_DOCS OFF)


# add_subdirectory(pybind11) *24*
add_subdirectory(glad)
add_subdirectory(glfw)
# I will add glfw as a find_package(glfw3 ...)


add_library(imgui STATIC
    "${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_draw.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_tables.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_widgets.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_demo.cpp"

    "${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends/imgui_impl_glfw.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends/imgui_impl_opengl3.cpp"
)

target_include_directories(imgui PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/imgui"
    "${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends"
)

target_link_libraries(imgui PUBLIC glfw)


# One 'lib' that keeps the libs for Software
add_library(projectLibs INTERFACE)
target_link_libraries(projectLibs INTERFACE
    glad
    glfw
    imgui
)
