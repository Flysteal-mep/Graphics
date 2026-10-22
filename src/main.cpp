#include <pybind11/embed.h>
#include <iostream>
#include <thread>

namespace py = pybind11;

int main()
{
    py::scoped_interpreter interpreter{};

    // Lets Python find the "python" package from the project root.
    py::module_::import("sys").attr("path").attr("insert")(0, ".");
    py::module_ graphics = py::module_::import("graphics");
    py::module_ app = py::module_::import("python.app");


    std::thread worker([&] {
        py::gil_scoped_acquire gil;
        app.attr("run")(graphics);
    });

    py::gil_scoped_release release;

    std::cout << "test\n";

    

    worker.join();

    return 0;
}
