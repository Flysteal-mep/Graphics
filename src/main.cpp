#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include "Butter.hpp"
#include <iostream>
#include <pybind11/embed.h>

namespace py = pybind11;


int main()
{
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


    // makes the pybind interpreter lifetime... I think
    py::scoped_interpreter interpreter{};

    // Runs python/app.py ^ & makes a cpp object
    py::module_::import("sys").attr("path").attr("insert")(0, "python");
    py::module_ app = py::module_::import("app");


    // mep ._.


    while (!glfwWindowShouldClose(window.Get()))
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.1, 0.1, 0.1, 0.1);

        // Runs the def Run() in python
        app.attr("Run")();

        glfwSwapBuffers(window.Get());
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}

PYBIND11_EMBEDDED_MODULE(SkyLands, m)
{

}
