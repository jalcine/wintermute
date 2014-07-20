#include "module.hh"
#include "module.hpp"

using Wintermute::Module;
using Wintermute::Module;
using Wintermute::ModulePrivate;

Module::Module(const Designation& designation) : d_ptr(new ModulePrivate)
{
  W_PRV(Module);
  d->designation = designation;
}

Module::Designation Module::designation() const
{
  W_PRV(Module);
  return d->designation;
}

bool Module::sendMessage(const Message& message) const
{
  // TODO Send a message using the MessageTunnel.
  return true;
}

bool Module::receiveMessage(const Message& message) const
{
  // TODO Send a message using the MessageTunnel.
  return true;
}

Module::~Module()
{
}
