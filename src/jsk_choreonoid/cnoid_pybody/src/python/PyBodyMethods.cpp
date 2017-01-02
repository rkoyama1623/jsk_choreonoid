#include "../BodyMethods.h"
#include "PyBodyMethods.h"
#include <boost/python.hpp>

namespace py = boost::python;
namespace np = boost::python::numpy;

BOOST_PYTHON_MODULE(BodyMethods)
{
  Py_Initialize();
  np::initialize();
  py::def("body_name", jskcnoid::body_name);
  py::def("angleVector", jskcnoid::py::angleVector);
};
