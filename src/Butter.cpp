#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include "Butter.hpp"
#include <iostream>
#include <stdexcept>
#include <utility>


CoalWindow::CoalWindow(const CoalWindowInfo& info)
    :m_info(info)
{}

CoalWindow::~CoalWindow()
{
    Destroy();
}

CoalWindow::CoalWindow(CoalWindow&& other) noexcept
    :m_window(std::exchange(other.m_window, nullptr)),
    m_info(std::move(other.m_info))
{}

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

