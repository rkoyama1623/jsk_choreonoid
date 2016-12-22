// #include <cnoid/PyUtil>
#include <cnoid/EigenTypes>
#include <cnoid/PyUtil>
#include <string>
#include <iostream>

std::string numpy_input(cnoid::Position position) {
  std::cout << position.linear() << std::endl;
  std::string s = "hoge";
  return s;
}

#include <boost/python.hpp>
namespace cnoid {
  void exportPyEigenTypes(); // cnoid/Util.so
}

BOOST_PYTHON_MODULE(numpy_sample)
{
  using namespace boost::python;

  cnoid::exportPyEigenTypes(); // set converter
  def("numpy_input", numpy_input);
}
