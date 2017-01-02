// #include <cnoid/PyUtil>

#include <boost/python.hpp>
#include "boost/python/numpy.hpp"
#include "boost/python/extract.hpp"
#include <cnoid/Body>
#include <stdexcept>
#include <iostream>
#include <algorithm>

namespace py = boost::python;
namespace np = boost::python::numpy;

namespace jskcnoid {
  namespace py {
    void angleVector(cnoid::BodyPtr &body, np::ndarray av) {
      int nd = av.get_nd(); // dimension of ndarray
      if (nd != 1)
        throw std::runtime_error("a must be 1-dimensional");
      if (av.get_dtype() != np::dtype::get_builtin<double>())
        throw std::runtime_error("a must be float64 array");
      auto shape = av.get_shape();
      auto strides = av.get_strides();

      for (int i = 0; i < shape[0]; ++i) {
        double q = *reinterpret_cast<double *>(av.get_data() + i * strides[0]);
        body->joint(i)->q() = q;
      }
    };
  }
}
