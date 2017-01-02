#include <iostream>
#include <boost/python.hpp>
#include "boost/python/numpy.hpp"
#include <boost/python/stl_iterator.hpp>
#include <string>

namespace py = boost::python;
namespace np = boost::python::numpy;

template< typename T >
inline
std::vector< T > to_std_vector( const py::object& iterable )
{
    return std::vector< T >( py::stl_input_iterator< T >( iterable ),
                             py::stl_input_iterator< T >( ) );
}

void get_list(const py::object& iterable) {
  std::vector< double > vec = to_std_vector< double >(iterable);
  // for (std::vector<double>::iterator i = vec.begin(); i != vec.end(); ++i)
  //   std::cout << *i << ' ';
  // std::cout << std::endl;
}

np::ndarray new_zero1(unsigned int N) {
  py::tuple shape = py::make_tuple(N);
  np::dtype dtype = np::dtype::get_builtin<double>();
  return np::zeros(shape, dtype);
}

BOOST_PYTHON_MODULE(numpy_array_sample)
{
  py::def("get_list", get_list);
  py::def("new_zero", new_zero1);
}
