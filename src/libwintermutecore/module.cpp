#include "module.hh"
#include "module.hpp"
#include "logging.hpp"

using Wintermute::Module;
using Wintermute::Module;
using Wintermute::ModulePrivate;

Module::Module(const Designation& designation) : d_ptr(new ModulePrivate)
{
  W_PRV(Module);
  d->designation = designation;
  wtrace("Module " + static_cast<string>(designation()) + " started.");
}

Module::Designation Module::designation() const
{
  W_PRV(Module);
  return d->designation;
}

bool Module::sendMessage(const Message& message) const
{
  wtrace("Module " + static_cast<string>(designation()) + " sending a message.");
  return true;
}

bool Module::receiveMessage(const Message& message) const
{
  wtrace("Module " + static_cast<string>(designation()) + " receiving a message.");
  return true;
}

Module::~Module()
{
}
