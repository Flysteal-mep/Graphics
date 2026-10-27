#pragma once
#include <string>
#include <filesystem>
#include "imgui.h"

struct GLFWwindow;
struct GLFWmonitor;


struct CoalWindowInfo
{
    int width = 500;
    int height = 500;
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

    int GetWidth() const
    {
        return m_info.width;
    }
    int GetHeight() const
    {
        return m_info.height;
    }


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

    // Delete the move constructor
    Shader(Shader&& other) = delete;
    // Delete the move assignment
    Shader& operator=(Shader&& other) = delete;

    void Use() const;
    void Delete();

private:
    unsigned int m_programShader = 0;

    unsigned int CompileShader(const std::filesystem::path& filePath, GLenum type);
    void Attach(unsigned int shaderID);
    void LinkProgram();

};


struct PaperImguiInfo
{
    ImGuiConfigFlags configFlags = ImGuiConfigFlags_None;
};

class PaperImgui
{
public:
    PaperImgui(const PaperImguiInfo& info);
    ~PaperImgui();

    // Delete the copy constructor
    PaperImgui(const PaperImgui&) = delete;
    // Delete the copy assignment opeerator (no more a = b)
    PaperImgui& operator=(const PaperImgui&) = delete;

    // Delete the move constructor
    PaperImgui(PaperImgui&& other) = delete;
    // Delete the move assignment
    PaperImgui& operator=(PaperImgui&& other) = delete;

    void Init(GLFWwindow* window);
    void NewFrame();
    void Render();
    void Shutdown();

private:
    PaperImguiInfo m_info;
};


class KiwiImage
{
public:
    KiwiImage(const std::filesystem::path& filePath);
    ~KiwiImage();

    // Delete the copy constructor
    KiwiImage(const KiwiImage&) = delete;
    // Delete the copy assignment opeerator (no more a = b)
    KiwiImage& operator=(const KiwiImage&) = delete;

    // Delete the move constructor
    KiwiImage(KiwiImage&& other) = delete;
    // Delete the move assignment
    KiwiImage& operator=(KiwiImage&& other) = delete;

    void Delete();
    unsigned int GetID() const;
    int GetWidth() const;
    int GetHeight() const;
    const std::filesystem::path& GetPath() const;

private:
    unsigned int m_glTextureID;

    int m_width;
    int m_height;
    int m_channels;
    std::filesystem::path m_filePath;
};
