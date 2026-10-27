#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include "Butter.hpp"
#include <iostream>


void WindowSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}


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


    glfwSetFramebufferSizeCallback(window.Get(), WindowSizeCallback);
    glfwSwapInterval(0);


    PaperImguiInfo imguiInfo{};
    imguiInfo.configFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    imguiInfo.configFlags |= ImGuiConfigFlags_NavEnableGamepad;
    imguiInfo.configFlags |= ImGuiConfigFlags_DockingEnable;

    PaperImgui imgui(imguiInfo);
    imgui.Init(window.Get());



    int sceneWidth = 800;
    int sceneHeight = 600;
    unsigned int fbo = 0;
    unsigned int texture = 0;
    unsigned int rbo = 0;

    // https://learnopengl.com/Advanced-OpenGL/Framebuffers
    // https://uysalaltas.github.io/2022/01/09/OpenGL_Imgui.html
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // https://docs.gl/gl4/glTexImage2D
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGB8,
        sceneWidth,
        sceneHeight,
        0,
        GL_RGB,
        GL_UNSIGNED_BYTE,
        nullptr
    );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glFramebufferTexture2D(
        GL_FRAMEBUFFER,
        GL_COLOR_ATTACHMENT0,
        GL_TEXTURE_2D,
        texture,
        0
    );

    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, sceneWidth, sceneHeight);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

    // It needs some data (color, depth or stencil) to be valid
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cerr << "Scene framebuffer is incomplete\n";
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);


    float vertices[6] =
    {
        -0.5, -0.5,
        0.5, -0.5,
        0.0,  0.5
    };

    unsigned int vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);

    unsigned int vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    Shader shader("src/shaders/vertex.glsl", "src/shaders/fragment.glsl");


    while (!glfwWindowShouldClose(window.Get()))
    {
        glfwPollEvents();

        {
            glBindFramebuffer(GL_FRAMEBUFFER, fbo);

            glViewport(0, 0, sceneWidth, sceneHeight);

            glClearColor(0.1, 0.1, 0.1, 1.0);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


            shader.Use();
            glBindVertexArray(vao);
            glDrawArrays(GL_TRIANGLES, 0, 3);
            glBindVertexArray(0);

            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }

        {
            glViewport(0, 0, window.GetWidth(), window.GetHeight());

            glClearColor(0.1, 0.1, 0.1, 1.0);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            imgui.NewFrame();
            ImGui::DockSpaceOverViewport(
                0,
                ImGui::GetMainViewport(),
                ImGuiDockNodeFlags_PassthruCentralNode
            );

            ImGui::Begin("Render scene");

            // https://github.com/ocornut/imgui/wiki/Image-Loading-and-Displaying-Examples
            ImGui::Image(
                texture,
                ImGui::GetContentRegionAvail(),
                // to make it right side up
                ImVec2(0, 1),
                ImVec2(1, 0)
            );

            ImGui::End();

            ImGui::ShowDemoWindow();

            imgui.Render();
        }

        glfwSwapBuffers(window.Get());
    }


    glfwTerminate();

    return 0;
}