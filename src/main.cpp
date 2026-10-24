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


    // Makes the pybind interpreter lifetime... I think
    py::scoped_interpreter interpreter{};
    // Imports sys to find and include the file
    py::module_::import("sys").attr("path").attr("insert")(0, "python");
    // Runs python/app.py & makes a cpp object
    py::module_ app = py::module_::import("app");


    Shader shader("src/shaders/vertex.glsl", "src/shaders/fragment.glsl");


    unsigned int vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);



    while (!glfwWindowShouldClose(window.Get()))
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.1, 0.1, 0.1, 0.1);


        shader.Use();
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);


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
