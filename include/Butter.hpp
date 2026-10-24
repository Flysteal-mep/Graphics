#pragma once
#include <string>
#include <filesystem>

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

    // delete the copy constructor
    CoalWindow(const CoalWindow&) = delete;
    // delete the copy assignment opeerator (no more a = b)
    CoalWindow& operator=(const CoalWindow&) = delete;


    CoalWindow(CoalWindow&& other) noexcept;
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

