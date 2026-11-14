#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <glad/gl.h>
#include <GLFW/glfw3.h>

char* ReadFile (const char* file_path)
{
    char* buffer = NULL;
    size_t file_size = 0;

    FILE* file = fopen(file_path, "rb");

    if (NULL == file)
    {
        perror("- Failed to open file");
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    file_size = ftell(file);
    rewind(file);

    buffer = malloc(file_size + 1);

    if (NULL == buffer)
    {
        perror("- File buffer is NULL");
        fclose(file);
        return NULL;
    }

    fread(buffer, 1, file_size, file);
    buffer[file_size] = '\0';

    if (ferror(file))
    {
        perror("- Failed to read file");
        fclose(file);
        return NULL;
    }

    fclose(file);

    return buffer;
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
