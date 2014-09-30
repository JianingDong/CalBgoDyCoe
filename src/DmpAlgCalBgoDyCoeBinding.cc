#include <boost/python.hpp>
#include "DmpAlgBgoDyCoe.h"

BOOST_PYTHON_MODULE(libDmpBgoDyCoe){
  using namespace boost::python;

  class_<DmpAlgBgoDyCoe,boost::noncopyable,bases<DmpVAlg> >("DmpAlgBgoDyCoe",init<>());
}
