#include <pybind11/embed.h>

#include "graphics.hpp"

namespace py = pybind11;


PYBIND11_EMBEDDED_MODULE(graphics, handle)
{
    handle.doc() = "The pybind to acces my app";
    handle.def("hello", []() {
        return "Hello from C++";
    });
    handle.def("create_window", &CreateWindow);
}
