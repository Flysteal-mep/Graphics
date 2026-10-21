#include <iostream>
#include <pybind11/pybind11.h>

namespace py = pybind11;


void test()
{
    std::cout << "test\n";
}

PYBIND11_MODULE(test, handle)
{
    handle.doc() = "this is a test doc";
    handle.def("create_window", &test);
}

int main()
{
    return 0;
}
