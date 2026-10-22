#include <iostream>
#include <string>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <pybind11/embed.h>

namespace py = pybind11;


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

    // set this to be moveble
    CoalWindow(const CoalWindow&) = delete;
    CoalWindow& operator=(const CoalWindow&) = delete;
    // same as above
    CoalWindow(CoalWindow&&) noexcept = default;
    CoalWindow& operator=(CoalWindow&&) noexcept = default;

    // (explicit) = only in a specific way it can be used
    explicit CoalWindow(const CoalWindowInfo& info)
        :m_window{nullptr}, m_info(info)
    {
    }

    bool Create()
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
            std::cerr << "- Failed to make glfw window, TODO: Update the error\n";
            return false;
        }

        glfwMakeContextCurrent(m_window);

        return true;
    }

    GLFWwindow* Get() const
    {
        if (!m_window)
        {
            return nullptr;
        }

        return m_window;
    }

    bool Destroy()
    {
        if (!m_window)
        {
            std::cerr << "- Warning window was null\n";
            return false;
        }

        glfwDestroyWindow(m_window);
        m_window = nullptr;

        return true;
    }

    bool SetContext()
    {
        if (!m_window)
        {
            return false;
        }

        glfwMakeContextCurrent(m_window);
        return true;
    }

private:
    GLFWwindow* m_window;
    CoalWindowInfo m_info;
};

int main()
{
    if (!glfwInit())
    {
        std::cerr << "- Failed to initialize GLFW\n";
        return 1;
    }

    // glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    // glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

    CoalWindowInfo windowInfo{};
    CoalWindow window(windowInfo);

    if (!window.Create())
    {
        glfwTerminate();
        return 1;
    }

    if (!gladLoadGL(glfwGetProcAddress))
    {
        std::cerr << "- Failed to initialize GLAD\n";
        window.Destroy();
        glfwTerminate();
        return 1;
    }

    glfwSwapInterval(0);
    // glViewport(0, 0, , );


    py::scoped_interpreter interpreter{};
    // makes the pybind interpreter lifetime... I think

    py::module_::import("sys").attr("path").attr("insert")(0, "python");
    py::module_ app = py::module_::import("app");
    // Runs python/app.py ^ & makes a cpp object


    while (!glfwWindowShouldClose(window.Get()))
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.1, 0.1, 0.1, 0.1);

        app.attr("Run")();
        // Runs the def Run() in python

        glfwSwapBuffers(window.Get());
        glfwPollEvents();
    }

    window.Destroy();
    glfwTerminate();

    return 0;
}


PYBIND11_EMBEDDED_MODULE(SkyLands, m)
{
    m.def("hello",
        []()
        {
            return "Hello test string";
        }
    );
    m.def("DrawDot",
        [](float x, float y, float size)
        {
            glPointSize(size);
            glBegin(GL_POINTS);
                glVertex2f(x, y); // Center of the screen
            glEnd();
        }
    );
}
