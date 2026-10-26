#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include "Butter.hpp"
#include <iostream>
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

// // Delete the copy constructor
// CoalWindow(const CoalWindow&) = delete;
// // Delete the copy assignment opeerator (no more a = b)
// CoalWindow& operator=(const CoalWindow&) = delete;

// https://en.cppreference.com/cpp/language/move_constructor !WARINUNG WHITE MODE!
CoalWindow::CoalWindow(CoalWindow&& other) noexcept
    :m_window(std::exchange(other.m_window, nullptr)),
    m_info(std::move(other.m_info))
{}

// https://en.cppreference.com/cpp/language/move_assignment
CoalWindow& CoalWindow::operator=(CoalWindow&& other) noexcept
{
    if (this == &other)
    {
        return *this;
    }

    Destroy();
    m_window = std::exchange(other.m_window, nullptr);
    m_info = std::move(other.m_info);

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

// // Delete the copy constructor
// Shader(const Shader&) = delete;
// // Delete the copy assignment opeerator (no more a = b)
// Shader& operator=(const Shader&) = delete;

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


// PaperImgui::PaperImgui() = default;
PaperImgui::~PaperImgui()
{
    Shutdown();
}

// // Delete the copy constructor
// PaperImgui(const PaperImgui&) = delete;
// // Delete the copy assignment opeerator (no more a = b)
// PaperImgui& operator=(const PaperImgui&) = delete;


void PaperImgui::Init(GLFWwindow* window)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

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
{
    unsigned char* data = stbi_load(filePath.c_str(), &m_width, &m_height, &m_channels, 4);

    if (data)
    {
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
}

KiwiImage::~KiwiImage()
{
    Delete();
}

void KiwiImage::Delete()
{
    glDeleteTextures(1, &m_glTextureID);
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
