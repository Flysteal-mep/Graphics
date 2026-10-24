// #include <pybind11/embed.h> *24*
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include "Butter.hpp"
#include <iostream>

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


    float data[6] =
    {
         0.0,  0.5,
        -0.5, -0.5,
         0.5, -0.5
    };

    unsigned int vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    // glBufferSubData I might need it in the future
    glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);

    // Helps to 'decode' the vbo
    unsigned int vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glEnableVertexAttribArray(0);
    // Tels how the data is stored
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);
    // 0 is location 2 is the floats per stride, ---, ---, the stride size,---


    Shader shader("src/shaders/vertex.glsl", "src/shaders/fragment.glsl");


    while (!glfwWindowShouldClose(window.Get()))
    {
        glfwPollEvents();


        // Runs the def Run() in python
        // app.attr("Run")(); *24*


        glClearColor(0.1, 0.1, 0.1, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        shader.Use();
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);


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
