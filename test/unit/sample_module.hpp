#ifndef WINTERMUTE_TEST_SAMPLE_MODULE
#define WINTERMUTE_TEST_SAMPLE_MODULE

#include "libwintermutecore/module.hpp"

class SampleModule : public Wintermute::Module
{
public:
  explicit SampleModule() :
    Wintermute::Module(Wintermute::Module::Designation("input", "test01.wintermute.in", 3001))
  {
  }
};

#endif
