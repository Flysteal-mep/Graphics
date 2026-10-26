// #include <pybind11/embed.h> *24*
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include "Butter.hpp"
#include <iostream>
#include "imgui.h"


// namespace py = pybind11; *24*


void WindowSizeCallback(GLFWwindow* window, int width, int height)
{
    glClearColor(0.1, 0.1, 0.1, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glViewport(0, 0, width, height);
}


int main()
{
    // Makes the pybind interpreter lifetime... I think
    // py::scoped_interpreter interpreter{}; *24*
    // Imports sys to find and include the file
    // py::module_::import("sys").attr("path").attr("insert")(0, "python"); *24*
    // Runs python/app.py & makes a cpp object
    // py::module_ app = py::module_::import("app"); *24*


    if (!glfwInit())
    {
        std::cerr << "- Failed to initialize GLFW\n";
        return 1;
    }

    // Source - https://stackoverflow.com/q/30148989
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    #endif

    CoalWindowInfo windowInfo{};

    CoalWindow window(windowInfo);
    window.Create();
    window.LoadGLAD();


    glfwSetFramebufferSizeCallback(window.Get(), WindowSizeCallback);
    glfwSwapInterval(0);


    PaperImgui imgui;
    imgui.Init(window.Get());


    KiwiImage image("src/MyImage01.jpg");


    while (!glfwWindowShouldClose(window.Get()))
    {
        glfwPollEvents();


        // Runs the def Run() in python
        // app.attr("Run")(); *24*


        imgui.NewFrame();

        ImGui::DockSpaceOverViewport();
        ImGui::ShowDemoWindow();

        ImGui::Begin("Render");

        // https://github.com/ocornut/imgui/wiki/Image-Loading-and-Displaying-Examples
        ImGui::Image(image.GetID(), ImVec2(image.GetWidth(), image.GetHeight()));

        ImGui::End();

        imgui.Render();


        glfwSwapBuffers(window.Get());
    }


    glfwTerminate();

    return 0;
}


// PYBIND11_EMBEDDED_MODULE(SkyLands, m) *24*
// { *24*
//     m.def("SetBackground", *24*
//         [](float r, float g, float b, float a) *24*
//         { *24*
//             glClearColor(r, g, b, a); *24*
//         } *24*
//     ); *24*
// } *24*
