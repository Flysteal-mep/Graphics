// #include <pybind11/embed.h> *24*
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include "Butter.hpp"
#include <iostream>
#include "imgui.h"

#define _CRT_SECURE_NO_WARNINGS
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// Simple helper function to load an image into a OpenGL texture with common settings
bool LoadTextureFromMemory(const void* data, size_t data_size, GLuint* out_texture, int* out_width, int* out_height)
{
    // Load from file
    int image_width = 0;
    int image_height = 0;
    unsigned char* image_data = stbi_load_from_memory((const unsigned char*)data, (int)data_size, &image_width, &image_height, NULL, 4);
    if (image_data == NULL)
    {
        return false;
    }

    // Create a OpenGL texture identifier
    GLuint image_texture;
    glGenTextures(1, &image_texture);
    glBindTexture(GL_TEXTURE_2D, image_texture);

    // Setup filtering parameters for display
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Upload pixels into texture
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
    stbi_image_free(image_data);

    *out_texture = image_texture;
    *out_width = image_width;
    *out_height = image_height;

    return true;
}

// Open and read a file, then forward to LoadTextureFromMemory()
bool LoadTextureFromFile(const char* file_name, GLuint* out_texture, int* out_width, int* out_height)
{
    FILE* f = fopen(file_name, "rb");
    if (f == NULL)
        return false;
    fseek(f, 0, SEEK_END);
    size_t file_size = (size_t)ftell(f);
    if (file_size == -1)
        return false;
    fseek(f, 0, SEEK_SET);
    void* file_data = IM_ALLOC(file_size);
    fread(file_data, 1, file_size, f);
    fclose(f);
    bool ret = LoadTextureFromMemory(file_data, file_size, out_texture, out_width, out_height);
    IM_FREE(file_data);
    return ret;
}


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


    PaperImgui imgui;
    imgui.Init(window.Get());


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
        // ImGui::Text("pointer = %x", image);
        // ImGui::Text("size = %d x %d", image_width, image_height);
        // ImGui::Image((ImTextureID)(intptr_t)image, ImVec2(image_width, image_height));
        int my_image_width = 0;
        int my_image_height = 0;
        unsigned int  my_image_texture = 0;
        bool ret = LoadTextureFromFile("src/MyImage01.jpg", &my_image_texture, &my_image_width, &my_image_height);
        IM_ASSERT(ret);
        ImGui::Image((ImTextureID)my_image_texture, ImVec2(my_image_width, my_image_height));

        ImGui::End();

        imgui.Render();


        glfwSwapBuffers(window.Get());
    }


    // During cleanup, while the GL context is still active:
    // glDeleteTextures(1, &image);


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
