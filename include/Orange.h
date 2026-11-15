#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <glad/gl.h>
#include <GLFW/glfw3.h>

typedef unsigned int  uint32_t;

void glfw_FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

char* ReadFile (const char* filePath)
{
    FILE* file = fopen(filePath, "rb");

    if (NULL == file)
    {
        perror("- Failed to open file");
        return NULL;
    }

    if (fseek(file, 0, SEEK_END))
    {
        perror("- Failed to seek file");
        fclose(file);
        return NULL;
    }

    size_t file_size = ftell(file);

    if (0 > file_size)
    {
        perror("- Failed to get file size");
        fclose(file);
        return NULL;
    }

    rewind(file);

    char* buffer = NULL;
    buffer = malloc(file_size + 1);

    if (NULL == buffer)
    {
        perror("- Failed to allocate file buffer");
        fclose(file);
        return NULL;
    }

    size_t bytes_read = fread(buffer, 1, file_size, file);

    if (file_size != bytes_read)
    {
        if (ferror(file))
        {
            perror("- Failed to read file");
        }
        else
        {
            fprintf(stderr, "- Unexpected end of file: %s\n", filePath);
        }

        free(buffer);
        fclose(file);
        return NULL;
    }

    buffer[bytes_read] = '\0';
    fclose(file);

    return buffer;
}

uint gl_CreateShader(const char* filePath, GLenum type)
{
    char* shaderSource = ReadFile(filePath);
    if (!shaderSource)
    {
        return 0;
    }

    uint shaderID = glCreateShader(type);
    if (!shaderID)
    {
        fprintf(stderr, "- Failed to create shader: %s\n", filePath);
        free(shaderSource);
        return 0;
    }

    glShaderSource(shaderID, 1, (const char**)&shaderSource, NULL);
    free(shaderSource);

    glCompileShader(shaderID);

    int success = 0;
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        char infoLog[512];

        glGetShaderInfoLog(shaderID, sizeof(infoLog), NULL, infoLog);
        fprintf(stderr, "- Shader compilation failed (%s):\n%s\n", filePath, infoLog);

        glDeleteShader(shaderID);
        return 0;
    }

    return shaderID;
}

int gl_LinkShaderProgram(uint programID)
{
    glLinkProgram(programID);

    int success = 0;
    glGetProgramiv(programID, GL_LINK_STATUS, &success);

    if (!success)
    {
        char infoLog[512];

        glGetProgramInfoLog(programID, sizeof(infoLog), NULL, infoLog);
        fprintf(stderr, "- Failed linking the progarm shader:\n%s\n", infoLog);

        return -1;
    }

    return programID;
}

int gl_SetUniformFloat(uint programID, const char* name, float value)
{
    int location = glGetUniformLocation(programID, name);
    if (-1 == location)
    {
        fprintf(stderr, "- Uniform not found or inactive: %s\n", name);
        return -1;
    }

    glUseProgram(programID);
    glUniform1f(location, value);
    return 0;
}

int gl_SetUniformInt(uint programID, const char* name, int value)
{
    int location = glGetUniformLocation(programID, name);
    if (-1 == location)
    {
        fprintf(stderr, "- Uniform not found or inactive: %s\n", name);
        return -1;
    }

    glUseProgram(programID);
    glUniform1i(location, value);
    return 0;
}
