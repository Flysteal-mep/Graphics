#include <stdio.h>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>

char* ReadFile(const char* filePath)
{
    FILE* file = fopen(filePath, "rb");
    if (!file)
    {
        perror(filePath);
        return NULL;
    }

    if (fseek(file, 0, SEEK_END) != 0)
    {
        fclose(file);
        return NULL;
    }

    long size = ftell(file);
    if (size < 0 || fseek(file, 0, SEEK_SET) != 0)
    {
        fclose(file);
        return NULL;
    }

    char* source = malloc((size_t)size + 1);
    if (!source)
    {
        fclose(file);
        return NULL;
    }

    size_t bytesRead = fread(source, 1, (size_t)size, file);
    fclose(file);

    if (bytesRead != (size_t)size)
    {
        free(source);
        return NULL;
    }

    source[size] = '\0';
    return source; // Caller must free this.
}

unsigned int gl_CreateShader(const char* filePath, GLenum type)
{
    char* shaderSource = ReadFile(filePath);
    if (!shaderSource)
    {
        return 0;
    }

    unsigned int shaderID = glCreateShader(type);
    glShaderSource(shaderID, 1, (const GLchar**)&shaderSource, NULL);
    free(shaderSource);

    glCompileShader(shaderID);

    GLint success;
    char infoLog[512];
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(shaderID, sizeof(infoLog), NULL, infoLog);
        fprintf(stderr, "- Shader compilation failed (%s):\n%s\n", filePath, infoLog);
    }

    return shaderID;
}

unsigned int gl_LinkShaderProgram(unsigned int programID)
{
    glLinkProgram(programID);

    GLint success;
    char infoLog[512];
    glGetProgramiv(programID, GL_LINK_STATUS, &success);

    if (!success)
    {
        glGetProgramInfoLog(programID, sizeof(infoLog), NULL, infoLog);
        fprintf(stderr, "- Failed linking the progarm shader:\n%s\n", infoLog);
    }

    return programID;
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
    // glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);

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

    if (!gladLoadGL(glfwGetProcAddress))
    {
        printf("error\n");
        return -1;
    }

    float vertices[] =
    {
         0.5,  0.5, 0.0,  0.8, 0.0, 0.0,
         0.5, -0.5, 0.0,  0.8, 0.0, 0.0,
        -0.5, -0.5, 0.0,  0.8, 0.0, 0.0,
        -0.5,  0.5, 0.0,  0.8, 0.0, 0.0,


    };
    unsigned int indices[] =
    {
        0, 1, 2,
        3, 0, 1
    };

    unsigned int vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);

    unsigned int vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(0 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    unsigned int ebo;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);



    unsigned int shaderProgram = 0;
    shaderProgram = glCreateProgram();

    unsigned int vertexShader = 0;
    vertexShader = gl_CreateShader("rsc/shaders/vertex.glsl", GL_VERTEX_SHADER);
    glAttachShader(shaderProgram, vertexShader);

    unsigned int fragmentShader = 0;
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

        glLineWidth(5.0f);
        glPointSize(15.0f);
        glUseProgram(shaderProgram);
        glBindVertexArray(vao);
        glDrawElements(GL_POINTS, sizeof(indices)/sizeof(indices[0]), GL_UNSIGNED_INT, 0);
        glDrawElements(GL_LINE_STRIP, sizeof(indices)/sizeof(indices[0]), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);


        glfwSwapBuffers(window);
    }


    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteProgram(shaderProgram);

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

