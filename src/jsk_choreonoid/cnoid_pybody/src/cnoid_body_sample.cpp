// #include <cnoid/PyUtil>
#include <cnoid/Body>
#include <string>

std::string body_name(cnoid::Body body) {
  std::string s = body.name();
  return s;
}

#include <boost/python.hpp>
BOOST_PYTHON_MODULE(cnoid_body_sample)
{
  using namespace boost::python;
  def("body_name", body_name);
}
