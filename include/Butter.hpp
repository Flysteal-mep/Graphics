#pragma once
#include <string>
#include <sstream>
#include <fstream>
#include <filesystem>
#include <iostream>

struct GLFWwindow;
struct GLFWmonitor;

struct CoalWindowInfo
{
    int width = 800;
    int height = 800;
    std::string title = "SkyLands";
    GLFWmonitor* monitor = nullptr;
    GLFWwindow* share = nullptr;
};

class CoalWindow
{
public:
    // (explicit) = only in a specific way it can be used
    explicit CoalWindow(const CoalWindowInfo& info);
    ~CoalWindow();

    // Delete the copy constructor
    CoalWindow(const CoalWindow&) = delete;
    // Delete the copy assignment opeerator (no more a = b)
    CoalWindow& operator=(const CoalWindow&) = delete;

    // Move constructor transfers ownership from another object
    CoalWindow(CoalWindow&& other) noexcept;
    // Move assignment transfers ownership to this object
    CoalWindow& operator=(CoalWindow&& other) noexcept;

    bool Create();
    void SetContext();
    GLFWwindow* Get() const;
    void Destroy();
    void LoadGLAD();


private:
    GLFWwindow* m_window;
    CoalWindowInfo m_info;
};


std::string ReadFile(const std::filesystem::path& filePath);

class Shader
{
public:
    Shader(const std::filesystem::path& vertexPath, const std::filesystem::path& fragmentPath);

    // Delete the copy constructor
    Shader(const Shader&) = delete;
    // Delete the copy assignment opeerator (no more a = b)
    Shader& operator=(const Shader&) = delete;

    void Use() const;

    void Delete();
private:
    unsigned int m_programShader = 0;

    unsigned int CompileShader(const std::filesystem::path& filePath, GLenum type);
    void Attach(unsigned int shaderID);
    void LinkProgram();

};
