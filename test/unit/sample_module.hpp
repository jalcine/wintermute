#include "libwintermutecore/module.hpp"

class SampleModule : public Wintermute::Module
{
public:
  explicit SampleModule() :
    Wintermute::Module(Wintermute::Module::Designation("input", "test01.wintermute.in", 3001))
  {
  }
};
