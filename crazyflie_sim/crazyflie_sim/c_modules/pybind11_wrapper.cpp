// pybind11_wrapper.cpp
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include "acc2quat.hpp"

namespace py = pybind11;

void acc2quat_wrapper(py::array_t<double> acc, double yaw, py::array_t<double> quat)
{
    auto acc_ptr = static_cast<double *>(acc.request().ptr);
    auto quat_ptr = static_cast<double *>(quat.request().ptr);

    acc2quat(acc_ptr, yaw, quat_ptr);
}

PYBIND11_MODULE(crazyflie_sil_c, m)
{
    m.doc() = "pybind11 example plugin"; // Optional module docstring
    m.def("acc2quat", &acc2quat_wrapper, "A function that multiplies two numbers",
          py::arg("acc"), py::arg("yaw"), py::arg("quat"));
}