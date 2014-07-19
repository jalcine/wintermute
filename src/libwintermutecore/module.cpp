#include "module.hh"
#include "module.hpp"

using Wintermute::Module;
using Wintermute::Module;
using Wintermute::ModulePrivate;
using Wintermute::DesignationPrivate;

Module::Module(const Designation& designation) : d_ptr(new ModulePrivate)
{
  W_PRV(Module);
  d->designation = designation;
}

Module::Module::Designation::Designation(const string& name, const string& domain, const pid_t& pid) :
  d_ptr (new DesignationPrivate)
{
  W_PRV(Designation);
  d->name = name;
  d->domain = domain;
  d->pid = pid;
}

Module::Designation::Designation() : d_ptr(new DesignationPrivate)
{
  W_PRV(Designation);
  d->name = "";
  d->domain = "";
  d->pid = getpid();
}

Module::Designation::Designation(const Designation& other) : d_ptr(new DesignationPrivate)
{
  W_PRV(Designation);
  d->clone(other.d_ptr);
}

string Module::Designation::name() const
{
  W_PRV(const Designation);
  return d->name;
}

string Module::Designation::domain() const
{
  W_PRV(const Designation);
  return d->domain;
}

pid_t Module::Designation::pid() const
{
  W_PRV(const Designation);
  return d->pid;
}

bool Module::Designation::isLocal() const
{
  W_PRV(const Designation);
  return d->pid == getpid();
}

bool Module::Designation::isNull() const
{
  W_PRV(const Designation);
  return d->domain.empty() && d->name.empty();
}

Module::Designation::~Designation()
{}

void DesignationPrivate::clone(const SharedPtr<DesignationPrivate>& other)
{
  this->pid = other->pid;
  this->name = other->name;
  this->domain = other->domain;
}
