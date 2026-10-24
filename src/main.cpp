// #include <pybind11/embed.h> *24*
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include "Butter.hpp"
#include <iostream>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

// namespace py = pybind11; *24*


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


    // Setup Dear ImGui context
    // https://github.com/ocornut/imgui/wiki/Getting-Started
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;     // IF using Docking Branch

    // Setup Platform/Renderer backends
    // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
    ImGui_ImplGlfw_InitForOpenGL(window.Get(), true);
    ImGui_ImplOpenGL3_Init();


    while (!glfwWindowShouldClose(window.Get()))
    {
        glfwPollEvents();


        // Runs the def Run() in python
        // app.attr("Run")(); *24*


        glClearColor(0.1, 0.1, 0.1, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);


        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Show demo window! :)
        ImGui::DockSpaceOverViewport();
        ImGui::ShowDemoWindow();

        // (Your code clears your framebuffer, renders your other stuff etc.)
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window.Get());
    }


    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

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
