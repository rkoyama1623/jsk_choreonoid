// #include <cnoid/PyUtil>
#include <iostream>
#include <boost/python.hpp>
#include <boost/python/stl_iterator.hpp>
#include <string>

namespace py = boost::python;

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

BOOST_PYTHON_MODULE(numpy_array_sample)
{
  using namespace boost::python;

  def("get_list", get_list);
}
