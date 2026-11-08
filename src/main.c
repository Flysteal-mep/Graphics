#include <stdio.h>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned int  uint;

typedef struct
{
    char name[32];
    float Kd[3];
} Material;

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

#define MATERIAL_SIZE 2
    Material materials[MATERIAL_SIZE];

    {
    uint materials_count = 0;

        FILE* file = fopen("rsc/obj/Data.mtl", "r");

        if (NULL == file)
        {
            return -1;
            perror("- Failed to open: mtl path");
        }

        char line[256] = {0};
        while (NULL != fgets(line, sizeof(line), file))
        {
            char type[32] = {0};
            if (1 != sscanf(line, "%31s", type))
            {
                continue;
            }

            if (0 == strcmp("newmtl", type))
            {
                sscanf(line, "%*s %31s",
                    materials[materials_count].name
                );

                materials_count++;
            }

            if (0 == strcmp("Kd", type))
            {
                sscanf(line, "%*s %f %f %f",
                    &materials[materials_count - 1].Kd[0],
                    &materials[materials_count - 1].Kd[1],
                    &materials[materials_count - 1].Kd[2]
                );
            }
        }

        fclose(file);
    }

#define BUFFER_SIZE 48
    float buffer[BUFFER_SIZE] = {0};
    const uint STRIDE = 6; // x y z  r g b

    {
    int active_material_index = 0; // default
    uint index = 0;
        FILE* file = fopen("rsc/obj/Data.obj", "r");

        if (NULL == file)
        {
            return -1;
            perror("- Failed to open: obj path");
        }

        char line[256] = {0};
        while (NULL != fgets(line, sizeof(line), file))
        {
            char type[32] = {0};
            if (1 != sscanf(line, "%31s", type))
            {
                continue;
            }

            if (0 == strcmp("usemtl", type))
            {
                char name[32] = {0};

                sscanf(line, "%*s %31s",
                    name
                );

                for (uint i = 0; i < MATERIAL_SIZE; i++)
                {
                    if (0 == strcmp(name, materials[i].name))
                    {
                        active_material_index = i;
                    }
                }
            }

            if (0 == strcmp("v", type))
            {
                sscanf(line, "%*s %f %f %f",
                    &buffer[index * STRIDE + 0],
                    &buffer[index * STRIDE + 1],
                    &buffer[index * STRIDE + 2]
                );
                buffer[index * STRIDE + 3] = materials[active_material_index].Kd[0];
                buffer[index * STRIDE + 4] = materials[active_material_index].Kd[1];
                buffer[index * STRIDE + 5] = materials[active_material_index].Kd[2];

                index++;
            }
        }

        fclose(file);
    }

    // for (int i = 0; i < BUFFER_SIZE/STRIDE; i++)
    // {
    //     int base = i * STRIDE;
    //     printf("%f %f %f  %f %f %f\n",
    //        buffer[base + 0], buffer[base + 1], buffer[base + 2],
    //        buffer[base + 3], buffer[base + 4], buffer[base + 5]
    //     );
    // } for debug

    unsigned int indices[] =
    {
        0, 1,
        1, 3,
        3, 2,
        2, 0,

        4, 5,
        5, 7,
        7, 6,
        6, 4
    };

    unsigned int vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(buffer), &buffer, GL_STATIC_DRAW);

    unsigned int vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, STRIDE * sizeof(float), (void*)(0 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, STRIDE * sizeof(float), (void*)(3 * sizeof(float)));
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
        glDrawElements(GL_LINES, sizeof(indices)/sizeof(indices[0]), GL_UNSIGNED_INT, 0);
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

