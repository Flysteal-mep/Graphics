#include "Orange.h"

int main(void)
{
    if (!glfwInit())
    {
        printf("error\n");
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);

    GLFWwindow* window = NULL;
    int window_width = 800;
    int window_height = 800;
    const char* window_title = "SkyLands";
    window = glfwCreateWindow(window_width, window_height, window_title, NULL, NULL);

    if (!window)
    {
        printf("error\n");
        return -1;
    }

    glfwMakeContextCurrent(window);
    // glfwSwapInterval(0);
    glfwSetFramebufferSizeCallback(window, glfw_FramebufferSizeCallback);

    if (!gladLoadGL(glfwGetProcAddress))
    {
        printf("error\n");
        return -1;
    }

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

//

    // x y z
    #define STRIDE 5

    // TODO Load from file or memory dynamically
    // And learn more on UVs.
    #define VERTICES_COUNT 15
    float vertices[VERTICES_COUNT] =
    {
         3.0, -1.0, 0.0,  2.0, 0.0,
        -1.0, -1.0, 0.0,  0.0, 0.0,
        -1.0,  3.0, 0.0,  0.0, 2.0
    };


    uint vbo = 0;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, VERTICES_COUNT * sizeof(float), &vertices, GL_STATIC_DRAW);

    uint vao = 0;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, STRIDE * sizeof(float), (void*)(0 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, STRIDE * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);


    uint shaderProgram_0 = 0;
    shaderProgram_0 = glCreateProgram();

    uint vertexShader = 0;
    vertexShader = gl_CreateShader("rsc/shaders/vertex.glsl", GL_VERTEX_SHADER);
    glAttachShader(shaderProgram_0, vertexShader);

    uint fragmentShader = 0;
    fragmentShader = gl_CreateShader("rsc/shaders/fragment.glsl", GL_FRAGMENT_SHADER);
    glAttachShader(shaderProgram_0, fragmentShader);

    gl_LinkShaderProgram(shaderProgram_0);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

//
    // TODO Clean up this part of code.

    #define TEXTURE_WIDTH 255
    #define TEXTURE_HEIGHT 255

    uint texture = 0;

    glGenTextures(1, &texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, TEXTURE_WIDTH, TEXTURE_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);

    glBindImageTexture(0, texture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);

    uint shaderProgram_1 = 0;
    shaderProgram_1 = glCreateProgram();

    uint computeShader = 0;
    computeShader = gl_CreateShader("rsc/shaders/compute.glsl", GL_COMPUTE_SHADER);
    glAttachShader(shaderProgram_1, computeShader);

    gl_LinkShaderProgram(shaderProgram_1);

    glDeleteShader(computeShader);


    glUseProgram(shaderProgram_0);
    gl_SetUniformInt(shaderProgram_0, "texture_u", 0);

//

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();


        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shaderProgram_1);

        // TODO Set a good value (I need to research on this to know if its alright)
        glDispatchCompute(TEXTURE_WIDTH, TEXTURE_HEIGHT, 1);
        glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

        glUseProgram(shaderProgram_0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glDrawArrays(GL_TRIANGLES, 0, VERTICES_COUNT);


        // TODO implement a ms counter

        glfwSwapBuffers(window);
    }


    glDeleteProgram(shaderProgram_0);
    glDeleteProgram(shaderProgram_1);

    glDeleteBuffers(1, &vao);
    glDeleteBuffers(1, &vbo);

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
