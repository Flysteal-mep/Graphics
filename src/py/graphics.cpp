#include <pybind11/embed.h>

#include "graphics.hpp"

namespace py = pybind11;


PYBIND11_EMBEDDED_MODULE(graphics, handle)
{
    handle.doc() = "The pybind to acces my API";
    handle.def("hello", []() {
        return "- Hello from C++";
    });

    handle.def("CreateDot", &CreateDot);

    py::class_<Cake>(handle, "Cake")
        .def(py::init<>())
        .def("Eat", &Cake::Eat)
        // .def("", &Cake::)
        ;
}

