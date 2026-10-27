#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include "Butter.hpp"
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <utility>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#define _CRT_SECURE_NO_WARNINGS
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

CoalWindow::CoalWindow(const CoalWindowInfo& info)
    :m_info(info)
{}

CoalWindow::~CoalWindow()
{
    Destroy();
}

// https://en.cppreference.com/cpp/language/move_constructor !WARINUNG WHITE MODE!
CoalWindow::CoalWindow(CoalWindow&& other) noexcept
    :m_window(std::exchange(other.m_window, nullptr)),
    m_info(std::move(other.m_info))
{}

// https://en.cppreference.com/cpp/language/move_assignment
CoalWindow& CoalWindow::operator=(CoalWindow&& other) noexcept
{
    if (this != &other)
    {
        Destroy();

        m_window = std::exchange(other.m_window, nullptr);
        m_info = std::move(other.m_info);
    }

    return *this;
}

bool CoalWindow::Create()
{
    m_window = glfwCreateWindow(
        m_info.width,
        m_info.height,
        m_info.title.c_str(),
        m_info.monitor,
        m_info.share
    );

    if (!m_window)
    {
        throw std::runtime_error("- Failed to create GLFW window");
    }

    glfwMakeContextCurrent(m_window);

    return true;
}

void CoalWindow::SetContext()
{
    if (!m_window)
    {
        throw std::runtime_error("- Can't set context of null window");
    }

    glfwMakeContextCurrent(m_window);
}

GLFWwindow* CoalWindow::Get() const
{
    if (!m_window)
    {
        std::cerr << "- Warning window IS null\n";
        return nullptr;
    }

    return m_window;
}

void CoalWindow::Destroy()
{
    if (!m_window)
    {
        std::cerr << "- Warning window was already null\n";
    }

    glfwDestroyWindow(m_window);
    m_window = nullptr;
}

void CoalWindow::LoadGLAD()
{
    if (!gladLoadGL(glfwGetProcAddress))
    {
        Destroy();
        glfwTerminate();

        throw std::runtime_error("- Failed to initialize GLAD");
    }
}


std::string ReadFile(const std::filesystem::path& filePath)
{
    std::ifstream file(filePath, std::ios::binary);

    if (!file)
    {
        std::cerr << "Failed to open file: " << filePath << '\n';
        return {};
    }

    std::ostringstream contents;
    contents << file.rdbuf();

    return contents.str();
}

Shader::Shader(const std::filesystem::path& vertexPath, const std::filesystem::path& fragmentPath)
    :m_programShader(glCreateProgram())
{
   if (!m_programShader)
   {
        throw std::runtime_error("- Failed to create shader program");
   }

    unsigned int vertexShader = 0;
    unsigned int fragmentShader = 0;

    try
    {
        vertexShader = CompileShader(vertexPath, GL_VERTEX_SHADER);
        Attach(vertexShader);

        fragmentShader = CompileShader(fragmentPath, GL_FRAGMENT_SHADER);
        Attach(fragmentShader);

        LinkProgram();
    }
    catch (...) // Catch ANY exception
    {
        if (vertexShader)
        {
            glDeleteShader(vertexShader);
        }
        if (fragmentShader)
        {
            glDeleteShader(fragmentShader);
        }

        Delete();
        throw;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Shader::Use() const
{
    glUseProgram(m_programShader);
}

void Shader::Delete()
{
    if (m_programShader)
    {
        glDeleteProgram(m_programShader);
        m_programShader = 0;
    }
}

unsigned int Shader::CompileShader(const std::filesystem::path& filePath, GLenum type)
{
    std::string source = ReadFile(filePath);
    const char* shaderSource = source.c_str();

    unsigned int shader = glCreateShader(type);
    if (!shader)
    {
        throw std::runtime_error("- Failed to create shader: " + filePath.string());
    }

    glShaderSource(shader, 1, &shaderSource, nullptr);
    glCompileShader(shader);

    int success = GL_FALSE;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        int logLength = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);

        std::string log(logLength, '\0');
        glGetShaderInfoLog(shader, logLength, nullptr, log.data());
        glDeleteShader(shader);

        throw std::runtime_error("- Failed to compile shader: " + filePath.string() + "\n" + log);
    }

    return shader;
}

void Shader::Attach(unsigned int shaderID)
{
    glAttachShader(m_programShader, shaderID);
}

void Shader::LinkProgram()
{
    glLinkProgram(m_programShader);

    int success = GL_FALSE;
    glGetProgramiv(m_programShader, GL_LINK_STATUS, &success);

    if (!success)
    {
        int logLength = 0;
        glGetProgramiv(m_programShader, GL_INFO_LOG_LENGTH, &logLength);

        std::string log(logLength, '\0');
        glGetProgramInfoLog(m_programShader, logLength, nullptr, log.data());

        throw std::runtime_error("- Failed to link shader program:\n" + log);
    }
}


PaperImgui::PaperImgui(const PaperImguiInfo& info)
    :m_info(info)
{
}

PaperImgui::~PaperImgui()
{
    Shutdown();
}

void PaperImgui::Init(GLFWwindow* window)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGui::GetIO().ConfigFlags |= m_info.configFlags;

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();
}

void PaperImgui::NewFrame()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void PaperImgui::Render()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void PaperImgui::Shutdown()
{
    if (ImGui::GetCurrentContext())
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }
}


KiwiImage::KiwiImage(const std::filesystem::path& filePath)
    :m_filePath(filePath)
{
    // ..filesystem::exists() apparently is not just for files
    if (!std::filesystem::is_regular_file(m_filePath))
    {
        std::cerr << "- File path is invalid: " << m_filePath << '\n';
    }

    // https://github.com/nothings/stb
    unsigned char* data = stbi_load(m_filePath.c_str(), &m_width, &m_height, &m_channels, 4);

    // https://stackoverflow.com/questions/49462889/stbi-load-doesnt-return-any-value
    if (!data)
    {
        std::cerr << "- Failed to load image with stbi" << '\n';
        std::cerr << stbi_failure_reason();
    }

    glGenTextures(1, &m_glTextureID);
    glBindTexture(GL_TEXTURE_2D, m_glTextureID);

    // https://docs.gl/gl4/glTexImage2D
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGBA8,
        m_width,
        m_height,
        0,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        data
    );

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);
}

KiwiImage::~KiwiImage()
{
    Delete();
}

void KiwiImage::Delete()
{
    if (m_glTextureID)
    {
        glDeleteTextures(1, &m_glTextureID);
        m_glTextureID = 0;
    }
}

unsigned int KiwiImage::GetID() const
{
    return m_glTextureID;
}

int KiwiImage::GetWidth() const
{
    return m_width;
}

int KiwiImage::GetHeight() const
{
    return m_height;
}

const std::filesystem::path& KiwiImage::GetPath() const
{
    return m_filePath;
}
