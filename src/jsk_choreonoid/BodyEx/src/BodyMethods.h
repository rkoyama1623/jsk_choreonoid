// #include <cnoid/PyUtil>
#include <cnoid/Body>
#include <string>

namespace jskcnoid {
  std::string body_name(cnoid::Body body) {
    std::string s = body.name();
    return s;
  }
}
