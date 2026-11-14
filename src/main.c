#include "Orange.h"

typedef unsigned int  uint32_t;

void glfw_FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

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
    glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);

    GLFWwindow* window = NULL;
    int window_width = 800;
    int window_height = 800;
    const char* window_title = "hello";
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


    uint shaderProgram = 0;
    shaderProgram = glCreateProgram();

    uint vertexShader = 0;
    vertexShader = gl_CreateShader("rsc/shaders/vertex.glsl", GL_VERTEX_SHADER);
    glAttachShader(shaderProgram, vertexShader);

    uint fragmentShader = 0;
    fragmentShader = gl_CreateShader("rsc/shaders/fragment.glsl", GL_FRAGMENT_SHADER);
    glAttachShader(shaderProgram, fragmentShader);

    gl_LinkShaderProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);


    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();


        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glDrawArrays(GL_TRIANGLES, 0, VERTICES_COUNT);


        glfwSwapBuffers(window);
    }

//

    glDeleteProgram(shaderProgram);

    glDeleteBuffers(1, &vao);
    glDeleteBuffers(1, &vbo);

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
